/*
 * A184731  a(n) = Sum_{k=0..n} C(n,k)^(k+1).
 * https://oeis.org/A184731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} C(n,k)^(k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;      /* accumulate the sum */
    seq_t binom = 1;     /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* (k+1) fits easily in seq_t for all n we consider */
        seq_t power = big_pow(binom, (seq_t)(k + 1));
        sum += power;

        /* compute C(n,k+1) from C(n,k) */
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
