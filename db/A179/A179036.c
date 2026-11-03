/*
 * A179036  Emirps whose only prime digits are 5's.
 * https://oeis.org/A179036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179036: Emirps whose only prime digits are 5's. */
/* The function enumerates decimal integers, keeps those whose digits
   contain no 2,3,7 and contain at least one 5, then tests primality
   of the number and its reverse (which must be a different prime).
   Results are cached so that calls can be in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/reverse_digits.h"
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

static seq_t *cache = NULL;      /* stored Emirps */
static size_t cache_len = 0;     /* number of stored terms */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t max_checked = 0;    /* last integer examined */

/* Grow the cache when capacity is exhausted. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true if x contains only digits 0-9 with no 2,3,7 and
   contains at least one digit 5. */
static bool digits_ok(seq_t x)
{
    uint8_t buf[200] = { 0 };
    size_t len = 0;
    get_digits(x, buf, &len);

    bool has5 = false;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (d == 2 || d == 3 || d == 7) return false;
        if (d == 5) has5 = true;
    }
    return has5;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Enumerate until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++max_checked;

        /* Quick digit test. */
        if (!digits_ok(max_checked))
            continue;

        /* Must be prime. */
        if (!is_prime_big(max_checked))
            continue;

        /* Reverse must be a distinct prime. */
        seq_t rev = reverse_digits(max_checked);
        if (rev == max_checked)          /* palindrome prime excluded */
            continue;
        if (!is_prime_big(rev))
            continue;

        /* Store the emirp. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = max_checked;
    }

    return cache[n - 1];
}
