/*
 * A063972  a(n) is the smallest m such that usigma(m) = n (or 0 if no such
 *          m).
 * https://oeis.org/A063972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least m such that the sum of unitary divisors of m equals n,
   or 0 if no such m exists.  The function works for any n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the sum of unitary divisors of a positive integer m.
   Since m ≤ 2·n + 100 with n ≤ 10000, m stays small (≤ ~20000),
   so trial division is efficient. */
static seq_t unitary_sigma(idx_t m) {
    seq_t result = 1;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
            }
            result *= (1 + power);
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        seq_t power = temp;       /* exponent is 1 */
        result *= (1 + power);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* undefined for n=0, but safe guard */

    /* Search m from 1 upwards.  In practice the minimal m is always < n,
       so a limit of 2·n + 100 is more than enough. */
    idx_t limit = n * 2 + 100;
    for (idx_t m = 1; m <= limit; ++m) {
        seq_t usig = unitary_sigma(m);
        if (usig == (seq_t)n) {
            return (seq_t)m;
        }
    }
    return 0;                     /* no m found */
}
