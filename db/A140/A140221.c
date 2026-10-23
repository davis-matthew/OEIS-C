/*
 * A140221  A number n is included if n is coprime to Sum_{k=1..n} d(k), where
 *          d(k) is the number of divisors of k.
 * https://oeis.org/A140221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first k ≥ 1 such that gcd(k, Σ_{i=1..k} d(i)) = 1 where
 *            d(i) is the number of divisors of i. Equivalent to the OEIS
 *            definition of A140221
 * Range:     u8 n <= 36, u16 n <= 4756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

#define MAX_GOOD_TERMS 200000   /* generous limit for n up to ~10000 */

/* Compute d(n) by trial division (works for small n). */
static seq_t divisor_count(idx_t n)
{
    idx_t tmp = n;
    seq_t res = 1;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (tmp > 1) {
        res *= 2;  /* remaining prime factor */
    }
    return res;
}

/* Static storage for good terms and running sum of d(i). */
static seq_t good_buf[MAX_GOOD_TERMS];
static size_t good_cnt = 0;
static idx_t   last_m  = 0;     /* largest m processed so far */
static seq_t   sumS    = 0;     /* Σ_{i=1..last_m} d(i) */

/* Extend the cache until at least n good terms exist. */
static void ensure_n_terms(size_t n)
{
    while (good_cnt < n) {
        ++last_m;
        seq_t d = divisor_count(last_m);
        sumS += d;
        if (gcd((seq_t)last_m, sumS) == 1) {
            if (good_cnt >= MAX_GOOD_TERMS) {
                /* In practice this should not happen for the
                   required n ≤ 10000.  Fail gracefully. */
                return;
            }
            good_buf[good_cnt++] = (seq_t)last_m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 but return 0 for safety */
    ensure_n_terms((size_t)n);
    return good_buf[n - 1];
}
