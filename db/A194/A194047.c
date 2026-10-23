/*
 * A194047  Inverse permutation to A194046; contains every positive integer
 *          exactly once.
 * https://oeis.org/A194047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by partitioning the indices into consecutive
 *            blocks of size 3+k (k = block number starting at 1). Let S(k) =
 *            1 + 2 + ... + (k+3) = k(k+7)/2 be the total number of terms up
 *            to and including block k. For a given n, find the smallest k
 *            such that S(k) ≥ n. Let offset = n - S(k-1) - 1...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find block k such that cumulative size S(k-1) < n ≤ S(k) */
    idx_t k = 1;          /* block number (1‑based) */
    idx_t cumulative = 0; /* sum of sizes of previous blocks */

    while (cumulative + k + 3 < n) {
        cumulative += k + 3;
        ++k;
    }

    /* offset within block k (0‑based) */
    idx_t offset = n - cumulative - 1;

    /* starting value of block k: b_k = 1 + (k-1)k/2 */
    seq_t start = 1 + (seq_t)(k - 1) * k / 2;

    if (offset <= 3) {
        seq_t off = (seq_t)offset;
        return start + off * k + off * (off + 1) / 2;
    } else {
        /* after the first three increments, each remaining step adds 1 */
        return start + 3 * k + 6 + (offset - 3);
    }
}
