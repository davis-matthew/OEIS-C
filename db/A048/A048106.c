/*
 * A048106  Number of unitary divisors of n (A034444) - number of non-unitary
 *          divisors of n (A048105).
 * https://oeis.org/A048106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(1+ω(n)) - τ(n) (number of non‑unitary divisors of n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t temp = (uint64_t)n;
    uint64_t omega = 0;          /* number of distinct prime factors ω(n) */
    seq_t tau = 1;               /* total divisor count τ(n) */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++omega;
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }

    if (temp > 1) {             /* remaining prime factor */
        ++omega;
        tau *= (seq_t)2;
    }

    seq_t pow = ((seq_t)1) << (omega + 1);  /* 2^(1+ω(n)) */
    return pow - tau;                      /* a(n) = 2^(1+ω) - τ */
}
