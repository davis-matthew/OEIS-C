/*
 * A104569  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product Q*R of the infinite lower triangular matrices Q = [1;
 *          1,3; 1,3,1; 1 3,1,3; ...] and R = [1; 1,1; 1,1,1; 1,1,1,1; ...].
 * https://oeis.org/A104569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i = smallest integer with i(i+1)/2 >= n, j =
 *            n - (i-1)i/2; T(i,j) = 2(i-j+1) if i,j have opposite parity,
 *            T(i,j) = 2(i-j)+1 if i,j are both odd, T(i,j) = 2(i-j)+3 if i,j
 *            are both even
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row index i such that i(i+1)/2 >= n */
    idx_t i = 1;
    while (((unsigned __int128)i * (i + 1)) / 2 < n)
        ++i;

    /* Compute starting index of this row: (i-1)i/2 */
    idx_t start = ((unsigned __int128)(i - 1) * i) / 2;

    /* Position within the row: 1 <= j <= i */
    idx_t j = n - start;
    idx_t diff = i - j;          /* diff = i - j */

    /* Determine parity cases and compute the value */
    if ((i & 1) != (j & 1)) {                /* opposite parity */
        return (seq_t)2 * ((seq_t)diff + 1);
    } else if ((i & 1) == 1) {               /* both odd */
        return (seq_t)2 * (seq_t)diff + 1;
    } else {                                 /* both even */
        return (seq_t)2 * (seq_t)diff + 3;
    }
}
