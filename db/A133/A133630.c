/*
 * A133630  Nonprime numbers k such that binomial(k+p,k) mod k = 1, where
 *          p=10.
 * https://oeis.org/A133630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 9719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9781 (2026-11-03)
 * Helpers:   gcd_u64, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the nth non‑prime integer k such that
   binomial(k+10,k) ≡ 1 (mod k).  The function
   searches candidates, tests the condition, and
   caches found terms to answer any n efficiently.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Compute C(k+10,10) mod k for a given non‑prime k. */
static seq_t binom10_mod_k(idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t res = (seq_t)1 % k_seq;

    /* Denominators 1..10, cancel with numerators. */
    uint64_t den[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Numerators from k+1 to k+10. */
    for (idx_t i = k + 1; i <= k + 10; ++i) {
        uint64_t num = (uint64_t)i;
        for (int j = 0; j < 10; ++j) {
            if (den[j] == 1) continue;
            uint64_t g = gcd_u64(num, den[j]);
            if (g > 1) {
                num /= g;
                den[j] /= g;
            }
        }
        /* num now represents the remaining numerator factor. */
        res = (res * (seq_t)num) % k_seq;
    }
    /* At this point all den[j] should be 1; binomial is an integer. */
    return res;
}

/* Euclidean gcd for 64‑bit unsigned integers. */

seq_t a(idx_t n)
{
    /* Cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  found  = 0;
    static idx_t  next_k = 4;   /* first candidate */

    while (found < n) {
        if (found == cap) {                 /* grow cache storage */
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) exit(1);
            cache = tmp;
            cap   = new_cap;
        }
        /* Find the next admissible non‑prime k. */
        for (;;) {
            if (!is_prime(next_k) &&
                binom10_mod_k(next_k) == (seq_t)1) {
                break;
            }
            ++next_k;
        }
        cache[found++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
