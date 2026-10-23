/*
 * A130265  Triangle read by rows: matrix product A007318 * A051340.
 * https://oeis.org/A130265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 126, u32 n <= 507, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130265 (triangle T(n,k)):
   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2),
            k = n - r(r+1)/2,
   and T(r,k) = (k+1)*C(r,k) + sum_{i=k+1}^r C(r,i). */
#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* row 0, position 0 */
        return (seq_t)1;

    idx_t r = find_row(n);
    idx_t k = n - (r * (r + 1) / 2);

    seq_t result = 0;
    seq_t bin = 1;                /* C(r,0) */
    const seq_t k1 = (seq_t)(k + 1);

    for (idx_t i = 0; i <= r; ++i) {
        if (i == k) {
            result += k1 * bin;   /* (k+1)*C(r,k) */
        }
        if (i > k) {
            result += bin;        /* add C(r,i) for i>k */
        }
        if (i < r) {
            /* compute next binomial C(r,i+1) */
            seq_t num = bin * (seq_t)(r - i);
            bin = num / (seq_t)(i + 1);   /* exact division */
        }
    }
    return result;
}
