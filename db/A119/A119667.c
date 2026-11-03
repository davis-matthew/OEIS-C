/*
 * A119667  Squares that contain multi-digit prime substrings.
 * https://oeis.org/A119667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 980 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119667: Squares that contain a multi‑digit prime substring
   The function generates squares in increasing order and keeps those
   that contain at least one decimal substring of length ≥ 2 that is
   prime.  The result is memoised so that any order of calls works. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return true if the decimal number x contains a prime substring of
   length at least 2.  get_digits gives digits least significant first. */
static bool has_prime_substring(seq_t x)
{
    /* A buffer big enough for any seq_t (up to 128 bits). */
    uint8_t digits[32];
    size_t len = 0;

    get_digits(x, digits, &len);
    if (len < 2)
        return false;   /* cannot contain a substring of length ≥ 2 */

    /* Reverse digits to have most significant digit first. */
    uint8_t rev[32];
    for (size_t i = 0; i < len; ++i)
        rev[i] = digits[len - 1 - i];

    for (size_t i = 0; i + 1 < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + rev[j];
            if (j - i + 1 >= 2) {
                if (is_prime_big(val))
                    return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of found squares. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_root = 1;   /* the next integer to square */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate squares until we have n+1 valid terms. */
    while (cache_len <= n) {
        idx_t r = next_root++;
        seq_t sq = (seq_t)r * (seq_t)r;
        if (has_prime_substring(sq)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = sq;
        }
    }

    return cache[n];
}
