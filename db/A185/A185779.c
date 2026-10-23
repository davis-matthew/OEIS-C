/*
 * A185779  Third accumulation array of Pascal's triangle (as a rectangle), by
 *          antidiagonals.
 * https://oeis.org/A185779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of T(i,j) where (i,j) are the antidiagonal
 *            coordinates of the 1‑based index n. T(n,k) = C(n+k+4, n+2) -
 *            (k+3)*(k+4)/2 - (k+2)*n*(k*n+n+3*k+7)/4 (for n,k ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 448, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* locate the antidiagonal that contains the n‑th element */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t s = high;
    idx_t prev = s * (s - 1) / 2;           /* elements before diagonal s */
    idx_t offset = n - prev - 1;            /* zero‑based index within the diagonal */
    idx_t i = offset + 1;                   /* row index (≥1) */
    idx_t j = s - offset;                   /* column index (≥1) */

    /* compute T(i,j) using the closed formula */
    idx_t n_binom = i + j + 4;
    idx_t k_binom = i + 2;
    seq_t A = binomial(n_binom, k_binom);

    seq_t n_seq = (seq_t)i;
    seq_t k_seq = (seq_t)j;

    seq_t B = ((k_seq + 3) * (k_seq + 4)) / 2;

    seq_t inner = k_seq * n_seq + n_seq + 3 * k_seq + 7;
    seq_t C = ((k_seq + 2) * n_seq * inner) / 4;

    return A - B - C;
}
