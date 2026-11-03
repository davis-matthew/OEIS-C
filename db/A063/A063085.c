/*
 * A063085  a(n) = usigma(n) - (phi(n) + d(n)), where usigma(n) is the sum of
 *          the unitary divisors of n and d(n) is the number of divisors of n.
 * https://oeis.org/A063085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = unitary_sigma(n) - (phi(n) + count_divisors(n))
 * Range:     i8 n <= 77, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 109 terms through n = 973 (2026-11-01)
 * Helpers:   num_divisors_idx_to_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/num_divisors_idx_to_idx.h"
#include "../utils/totient.h"

static inline seq_t unitary_sigma(idx_t n)
{
    /* Sum of unitary divisors of n.
       For each prime power p^e dividing n,
       multiply the result by (1 + p^e). */
    if (n == 0) return 0;                 /* unused for n >= 1 */
    seq_t res = 1;                        /* product over prime factors */
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t p_pow = 1;
            do {
                m /= p;
                p_pow *= (seq_t)p;
            } while (m % p == 0);
            res *= ((seq_t)1 + p_pow);
        }
    }
    if (m > 1) {                          /* remaining prime factor > sqrt(n) */
        res *= ((seq_t)1 + (seq_t)m);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 by sequence definition */
    seq_t us = unitary_sigma(n);
    seq_t ph = totient(n);
    seq_t d  = num_divisors_idx_to_idx(n);
    return us - (ph + d);
}
