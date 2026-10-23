/*
 * A198309  Moore lower bound on the order of a (9,g)-cage.
 * https://oeis.org/A198309
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Let i = n/2 if n is even Let i = (n-1)/2 if n is odd Then S =
 *            (8^i - 1)/7 can be computed by the recursion S_{k+1} = 8 * S_k +
 *            1 (S_0 = 0) a(n) = 2 * S if n even a(n) = 1 + 9 * S if n odd
 *            (This works for all n ≥ 3, and for any seq_t that can represent
 *            the result without overflow.)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* undefined below offset */

    idx_t i = (n & 1) ? (n - 1) / 2 : n / 2; /* i = floor(n/2) if even, (n-1)/2 if odd */
    seq_t s = 0;                          /* S = (8^i - 1)/7, computed iteratively */
    for (idx_t k = 0; k < i; ++k)
        s = s * (seq_t)8 + (seq_t)1;       /* S_{k+1} = 8*S_k + 1 */

    if (n & 1)  /* odd */
        return (seq_t)1 + (seq_t)9 * s;
    else        /* even */
        return s * (seq_t)2;
}
