/*
 * A135086  Triangle, antidiagonals of an array formed by A000012 *
 *          A130321(transform).
 * https://oeis.org/A135086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the antidiagonals of the * infinite
 *            array A[r][c] defined by * * A[r][c] = 2^c – 1 for c ≤ r, *
 *            A[r][c] = (2^r – 1) * 2^(c‑r) for c > r. * * The mapping from
 *            index n (offset 1) to array coordinates is: * * Let k be the
 *            smallest integer with k(k+1)/2 ≥ n. * prev = k(k‑1...
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1; 0 is unused */

    /* Find the antidiagonal index k such that k(k+1)/2 ≥ n */
    idx_t k   = 1;
    idx_t sum = 1;                       /* triangular number T_k */
    while (sum < n) {
        ++k;
        sum += k;
    }

    idx_t prev   = sum - k;              /* T_{k-1} = k(k-1)/2 */
    idx_t offset = n - prev - 1;         /* zero‑based offset on k‑th antidiagonal */
    idx_t r = k - offset;
    idx_t c = offset + 1;

    if (c <= r) {
        /* A[r][c] = 2^c – 1 */
        seq_t pow2_c = big_pow((seq_t)2, (seq_t)c);
        return pow2_c - (seq_t)1;
    } else {
        /* A[r][c] = (2^r – 1) * 2^(c – r) */
        seq_t pow2_r   = big_pow((seq_t)2, (seq_t)r);
        seq_t term     = pow2_r - (seq_t)1;
        seq_t pow2_diff = big_pow((seq_t)2, (seq_t)(c - r));
        return term * pow2_diff;
    }
}
