/*
 * A157911  Nonprimes whose digits are all cubes.
 * https://oeis.org/A157911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   enumerate all non‑prime numbers whose digits are 0, 1 or 8, * in
 *            increasing order. The first terms are 0, 1, 8, 10, 18, … (see
 *            OEIS A157911)
 * Range:     u8 n <= 15, u16 n <= 146, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 64 terms through n = 1998 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */
#include <stdint.h>
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

/* Cache of the non‑prime cube‑digit numbers found so far. */
static seq_t *cache      = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint8_t next_len   = 1;   /* length of the numbers to generate next */

/* Recursively generate all numbers of a given length 'len' using
 * the digits 0, 1, 8. When a full length number is reached it is
 * tested for primality; if it is not prime it is stored in the cache. */
static void generate_len(uint8_t len, seq_t curr, uint8_t pos)
{
    static const uint8_t digits[3] = {0, 1, 8};

    if (pos == len) {
        /* Store the number if it is not prime. */
        if (!is_prime_big(curr)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();   /* out of memory */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = curr;
        }
        return;
    }

    for (unsigned i = 0; i < 3; ++i) {
        uint8_t d = digits[i];
        /* Skip a leading zero for numbers longer than one digit. */
        if (pos == 0 && len > 1 && d == 0)
            continue;
        generate_len(len, curr * 10 + d, pos + 1);
    }
}

/* Generate terms until the cache contains at least 'need' items. */
static void ensure_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        generate_len(next_len, 0, 0);
        ++next_len;
    }
}

/* Return the nth term (1‑based) of OEIS A157911. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1, safe guard */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
