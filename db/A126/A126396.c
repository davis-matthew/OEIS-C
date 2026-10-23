/*
 * A126396  Number of base 9 n-digit numbers with adjacent digits differing by
 *          two or less.
 * https://oeis.org/A126396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126396: count of base‑9 n‑digit strings whose adjacent digits differ by at most 2
   (a(0)=1, a(1)=9).  Computed by a linear DP that stores counts for strings ending in each digit. */
seq_t a(idx_t n)
{
    /* Handle trivial cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* f[d] holds the number of length‑i strings ending with digit d. */
    seq_t f[9];
    seq_t next[9];
    idx_t i, d, e;

    for (d = 0; d < 9; ++d) f[d] = (seq_t)1;   /* length 1 */

    /* Build lengths 2 .. n */
    for (i = 2; i <= n; ++i) {
        for (d = 0; d < 9; ++d) {
            /* Digit d can be preceded by any digit e with |e‑d| ≤ 2. */
            idx_t lo = (d > 2) ? d - 2 : 0;      /* max(0, d-2) */
            idx_t hi = (d < 7) ? d + 2 : 8;      /* min(8, d+2) */

            seq_t sum = (seq_t)0;
            for (e = lo; e <= hi; ++e) sum += f[e];
            next[d] = sum;
        }
        /* Copy next into f for the next iteration. */
        for (d = 0; d < 9; ++d) f[d] = next[d];
    }

    /* Sum over all ending digits gives the answer. */
    seq_t total = (seq_t)0;
    for (d = 0; d < 9; ++d) total += f[d];
    return total;
}
