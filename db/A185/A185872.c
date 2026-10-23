/*
 * A185872  Accumulation array of the (odd,odd)-polka dot array A185868, by
 *          antidiagonals.
 * https://oeis.org/A185872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n.
 *            T(i,j) = (i*j/6)*(4*i^2 + 6*i*j + 4*j^2 - 3*i - 9*j + 4)
 * Range:     u8 n <= 17, u16 n <= 334, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d such that (d+1)(d+2)/2 >= n. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n)
        ++d;

    /* Position inside the diagonal.  Elements up to diagonal d-1
       occupy d*(d+1)/2 positions. */
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev - 1;

    /* Translate to matrix coordinates (i,j). */
    idx_t i = offset + 1;
    idx_t j = d + 1 - offset;

    /* Compute T(i,j) using 64‑bit intermediate arithmetic
       to avoid overflow on small seq_t types. */
    unsigned long long ii = (unsigned long long)i;
    unsigned long long jj = (unsigned long long)j;
    unsigned long long poly =
        4ULL * ii * ii           /* 4*i^2 */
        + 6ULL * ii * jj         /* 6*i*j */
        + 4ULL * jj * jj         /* 4*j^2 */
        - 3ULL * ii              /* -3*i */
        - 9ULL * jj              /* -9*j */
        + 4ULL;                  /* +4 */
    unsigned long long prod = ii * jj * poly;

    /* Divide by 6; the result fits in the destination type. */
    return (seq_t)(prod / 6ULL);
}
