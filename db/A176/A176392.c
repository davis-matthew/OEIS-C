/*
 * A176392  Triangle read by rows: T(n,k) = 2 - (1 + k)^k + (1 + n)^n - (1 - k
 *          + n)^(n - k).
 * https://oeis.org/A176392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 2 - (1 + k)^k + (1 + r)^r - (1 - k +
 *            r)^(r - k). This returns the nth term (0‑based) of the triangle
 *            read by rows
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Determine row r and column k from the row‑major index n. */
    idx_t cum = 0;          /* cumulative entries before current row */
    idx_t len = 1;          /* length of the current row (r+1) */
    while (cum + len <= n) {
        cum += len;
        len++;
    }
    idx_t r = len - 1;      /* row index (0‑based) */
    idx_t k = n - cum;      /* column index in that row */

    /* Compute the three powers using the provided big_pow routine. */
    seq_t term1 = big_pow((seq_t)(1 + k), (seq_t)k);
    seq_t term2 = big_pow((seq_t)(1 + r), (seq_t)r);
    seq_t term3 = big_pow((seq_t)(1 + r - k), (seq_t)(r - k));

    /* Combine the terms.  All arithmetic is performed in seq_t which is
       unsigned; the sequence guarantees the final result is non‑negative
       within the type range. */
    seq_t result = (seq_t)2 - term1 + term2 - term3;
    return result;
}
