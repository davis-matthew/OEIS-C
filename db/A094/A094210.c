/*
 * A094210  Numbers k such that k^2 + 3k + 1 is a prime.
 * https://oeis.org/A094210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that k^2 + 3k + 1 is prime. (OEIS A094210)
 * Range:     u8 n <= 11, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9836 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    /* static state: last k examined and cache of found k's */
    static idx_t last_checked = 0;      /* last k examined (>= 0) */
    static idx_t *terms = NULL;         /* array of found k's (1‑based indices) */
    static size_t terms_cap = 0;        /* allocated capacity of terms array */
    static size_t terms_len = 0;        /* number of terms stored */

    /* Grow the cache until it contains at least n terms */
    while (terms_len < n) {
        last_checked++;                   /* consider next integer k */

        /* compute k^2 + 3k + 1 in seq_t to avoid overflow */
        seq_t val = (seq_t)last_checked * (seq_t)last_checked
                    + (seq_t)3 * (seq_t)last_checked + (seq_t)1;

        if (is_prime_big(val)) {
            /* ensure capacity of the array */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                if (!tmp) abort();            /* allocation failed: halt */
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_checked;
        }
    }

    /* sequence is 1‑based, array is 0‑based */
    return (seq_t)terms[n - 1];
}
