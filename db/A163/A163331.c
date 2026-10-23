/*
 * A163331  Inverse permutation to A163330.
 * https://oeis.org/A163331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((x + y)² + x + 3·y) / 2 where n = Σ d_k·3^k (base‑3
 *            digits d_k), and x = Σ_{k odd} d_k·3^{(k-1)/2}, y = Σ_{k even}
 *            d_k·3^{k/2}
 * Range:     u8 n <= 132, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Source: OEIS A163331 – inverse permutation to A163330. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute x and y by interleaving base‑3 digits of n */
    idx_t   idx   = 0;          /* index of the current digit (LSB = 0)   */
    idx_t   tmp   = n;          /* copy of n for digit extraction        */
    seq_t   x     = 0;          /* the 'odd‑position' number             */
    seq_t   y     = 0;          /* the 'even‑position' number            */
    seq_t   pow_x = 1;          /* 3^(number of odd digits seen so far)   */
    seq_t   pow_y = 1;          /* 3^(number of even digits seen so far)  */

    while (tmp > 0) {
        idx_t digit = tmp % 3;  /* current base‑3 digit          */
        tmp      /= 3;

        if ((idx & 1) == 0) {   /* even index → part of y       */
            y      += (seq_t)digit * pow_y;
            pow_y   *= 3;
        } else {                /* odd index → part of x        */
            x      += (seq_t)digit * pow_x;
            pow_x   *= 3;
        }
        ++idx;
    }

    /* compute a(n) = ((x + y)² + x + 3·y) / 2 */
    seq_t sum       = x + y;
    seq_t numerator = sum * sum + x + 3 * y;
    return numerator / 2;
}
