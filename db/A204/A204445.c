/*
 * A204445  Symmetric matrix: f(i,j)=floor[(i+j+4)/4]-floor[(i+j+1)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 unless the antidiagonal index s(n) ≡ 3 (mod 4); s(n) is
 *            the smallest integer ≥ 2 with (s−1)*s/2 ≥ n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* not used (offset 1), but safe */
        return (seq_t)0;

    idx_t s   = 2;   /* current diagonal index (sum i+j) */
    idx_t cum = 0;   /* cumulative number of terms up to previous diagonal */

    /* accumulate counts of terms per diagonal until we reach or pass n */
    while (1) {
        cum += s - 1;            /* each diagonal s has (s-1) entries */
        if (cum >= n)
            break;                /* found the diagonal containing a(n) */
        ++s;
    }

    /* the value on any diagonal depends only on s modulo 4 */
    return ((s & 3) == 3) ? (seq_t)0 : (seq_t)1;
}
