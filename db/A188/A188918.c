/*
 * A188918  Alternate partial sums of binomial(2n,n)*binomial(3n,n) (A006480).
 * https://oeis.org/A188918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} (-1)^(n-k) * binom(2*k,k) * binom(3*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute the OEIS sequence A188918.  It is valid for both C11 and C++17
 * and works for any seq_t/idx_t types supplied by the build system. */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binom(2 * k, k);
        seq_t t2 = binom(3 * k, k);
        seq_t term = t1 * t2;
        if (((n - k) & 1) != 0) {
            term = -term;   /* apply the (-1)^(n-k) factor */
        }
        sum += term;
    }
    return sum;
}
