/*
 * A185786  Third accumulation array of A107985, by antidiagonals.
 * https://oeis.org/A185786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(n,k) = (n+k+3)*C(n+3,4)*C(k+3,4)/5 and
 *            (i,j) are determined by ordering the 2‑D array along
 *            antidiagonals (sum n+k fixed)
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 924, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* not part of the sequence */

    /* Find smallest s such that (s-1)*s/2 >= n   (s = n+k) */
    idx_t low = 1, high = 2;
    while ( ( (unsigned __int128)high*(high-1)/2 ) < (unsigned __int128)n )
        high <<= 1;          /* high *= 2 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                     /* s = n + k of the antidiagonal */

    /* Number of terms before this diagonal */
    unsigned __int128 prevTri = (unsigned __int128)(s-2) * (s-1) / 2;
    idx_t prev = (idx_t)prevTri;

    /* Position within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;
    idx_t i = offset + 1;              /* row index (n in T) */
    idx_t j = s - i;                   /* column index (k in T) */

    /* Compute T(i,j) = (i+j+3)*C(i+3,4)*C(j+3,4)/5 */
    seq_t bin1 = binom(i + 3, 4);      /* C(i+3,4) */
    seq_t bin2 = binom(j + 3, 4);      /* C(j+3,4) */
    seq_t sum = (seq_t)(i + j + 3);    /* i + j + 3 */
    seq_t product = sum * bin1;        /* (i+j+3)*C(i+3,4)   */
    product *= bin2;                   /* times C(j+3,4)      */
    return product / (seq_t)5;         /* divide by 5 (exact) */
}
