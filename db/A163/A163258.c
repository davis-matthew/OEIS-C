/*
 * A163258  Fractal sequence of the interspersion A163257.
 * https://oeis.org/A163258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via segment decomposition with recursive mapping
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the segment index k (0‑based) that contains position n. */
    idx_t k = 0;
    while ((unsigned __int128)(k + 1) * (k + 4) < n)
        ++k;

    /* Index within the segment (0‑based). */
    idx_t start = (unsigned __int128)k * (k + 3) + 1;
    idx_t i = n - start;

    /* Iteratively map down to the base segment. */
    while (1) {
        /* Base segment contains 1, 2, 3, 4 */
        if (k == 0) {
            if (i == 0) return (seq_t)1;
            if (i == 1) return (seq_t)2;
            if (i == 2) return (seq_t)3;
            return (seq_t)4;          /* i == 3 */
        }

        if (i == 0) return (seq_t)1;
        if (i == 1) return (seq_t)2;

        /* Handle the two new numbers inserted in this segment. */
        if (i == 2)                     /* odd number 2k+3 */
            return (seq_t)(2 * k + 3);
        if (i == k + 3)                 /* even number 2k+4 */
            return (seq_t)(2 * k + 4);

        /* Map the remaining position to the previous segment. */
        idx_t prev_k = k - 1;        /* previous segment index */
        idx_t prev_b = k + 2;        /* (prev_k + 3) */
        idx_t i_prev;

        if (i <= prev_b)
            i_prev = i - 1;          /* indices 3..k+2 map to 2..k+1 */
        else
            i_prev = i - 2;          /* indices k+4.. end map to k+2.. */

        k = prev_k;
        i = i_prev;
    }
}
