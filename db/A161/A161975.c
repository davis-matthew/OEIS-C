/*
 * A161975  Sum_{x=sum of prime factors of n-th composite..n-th composite} x.
 * https://oeis.org/A161975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k = sum_of_prime_factors(C_n)}^{C_n} k where C_n is
 *            the n‑th composite number. The sum of consecutive integers from
 *            S to C is (C - S + 1) * (C + S) / 2
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* for safety; OEIS offset is 1 */
        return (seq_t)0;

    idx_t   count = 0;
    uint64_t m = 4;                  /* first composite */

    while (true) {
        if (!is_prime(m)) {          /* m is composite */
            ++count;
            if (count == n) {        /* found the n-th composite */
                /* sum of prime factors of m */
                seq_t sum_pf = 0;
                uint64_t t = m;
                for (uint64_t p = 2; p <= t / p; ++p) {
                    while (t % p == 0) {
                        sum_pf += (seq_t)p;
                        t /= p;
                    }
                }
                if (t > 1)
                    sum_pf += (seq_t)t;

                seq_t C = (seq_t)m;
                seq_t S = sum_pf;
                seq_t term = ( (C - S + (seq_t)1) * (C + S) ) / (seq_t)2;
                return term;
            }
        }
        ++m;
    }
}
