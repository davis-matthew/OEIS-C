/*
 * A218125  Number of 8-ary sequences with primitive period n.
 * https://oeis.org/A218125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 8^d * mu(n/d) for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* divisor d1 = i */
        idx_t d1 = i;
        int mu1 = mu_of(n / d1);
        if (mu1 != 0) {
            seq_t p = big_pow((seq_t)8, (seq_t)d1);
            if (mu1 > 0)
                sum += p;
            else
                sum -= p;
        }

        /* divisor d2 = n / i */
        idx_t d2 = n / i;
        if (d2 != d1) {
            int mu2 = mu_of(n / d2); /* n / d2 = i */
            if (mu2 != 0) {
                seq_t p = big_pow((seq_t)8, (seq_t)d2);
                if (mu2 > 0)
                    sum += p;
                else
                    sum -= p;
            }
        }
    }

    return sum;
}
