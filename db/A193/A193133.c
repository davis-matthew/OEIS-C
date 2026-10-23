/*
 * A193133  Number of spanning trees in the n-crown graph.
 * https://oeis.org/A193133
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n-2)^(n-1) * (n-1) * n^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3u) return 0;

    seq_t nm1 = (seq_t)n - 1;          /* n-1 */
    seq_t nm2 = (seq_t)n - 2;          /* n-2 */

    seq_t pow1 = big_pow(nm2, nm1);                 /* (n-2)^(n-1) */
    seq_t pow2 = big_pow((seq_t)n, (seq_t)n - 2);   /*  n^(n-2) */

    seq_t res = pow1 * pow2;   /* multiply the two powers */
    res = res * nm1;           /* multiply by (n-1) */

    return res;
}
