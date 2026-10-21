/*
 * A202670  Symmetric matrix based on A000290 (the squares), by antidiagonals.
 * https://oeis.org/A202670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{t=1}^{min(i,j)} t^2 * (|i-j| + t)^2, where i and j
 *            are obtained from the antidiagonal enumeration. Specify i and j:
 *            let d be the smallest integer with d*(d+1)/2 >= n let Cprev =
 *            d*(d-1)/2 let offset = n - Cprev - 1 (0‑based on the d‑th
 *            antidiagonal) i = offset + 1 j = d - offset Then...
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset
        Zero is not a valid term, but returning 0 is safe. */
        return 0;

    /* Find the diagonal index d: smallest d with d*(d+1)/2 >= n. */
    long double approx = sqrt((long double)8 * (long double)n + 1);
    idx_t d = (idx_t)((approx - 1.0L) / 2.0L);   /* floor */
    while (((__int128)d * (d + 1)) / 2 < n) d++;
    while (d > 0 && (((__int128)(d - 1) * d) / 2) >= n) d--;

    /* Cprev = d*(d-1)/2 fits in idx_t because it is < n */
    __int128 tmp = (__int128)d * (d - 1) / 2;
    idx_t Cprev = (idx_t)tmp;
    idx_t offset = n - Cprev - 1;             /* 0‑based within diagonal */

    /* i,j for the n‑th entry */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* p = min(i,j), delta = |i-j| */
    seq_t p = (i < j) ? (seq_t)i : (seq_t)j;
    seq_t delta = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);

    /* helper sequences */
    seq_t p_seq = p;
    seq_t two_p = (seq_t)2 * p_seq;

    /* Σ t^2 = p(p+1)(2p+1)/6 */
    seq_t sum2 = p_seq * (p_seq + 1) * (two_p + 1) / 6;

    /* Σ t^3 = (p(p+1)/2)^2 */
    seq_t tmp_a = p_seq * (p_seq + 1) / 2;
    seq_t sum3 = tmp_a * tmp_a;

    /* Σ t^4 = p(p+1)(2p+1)(3p^2+3p-1)/30 */
    seq_t p_sq = p_seq * p_seq;
    seq_t temp = (seq_t)3 * p_sq + (seq_t)3 * p_seq - 1;
    seq_t sum4 = p_seq * (p_seq + 1) * (two_p + 1) * temp / 30;

    /* compute the result: Σ t^4 + 2*delta*Σ t^3 + delta^2*Σ t^2 */
    seq_t result = sum4 + (seq_t)2 * delta * sum3 + delta * delta * sum2;
    return result;
}
