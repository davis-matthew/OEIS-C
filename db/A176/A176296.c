/*
 * A176296  Irregular triangle read by rows: eigenvalues of Laplacian of
 *          parity's landscape graph.
 * https://oeis.org/A176296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, more
 * Formula:   a(n) is read rowwise from the irregular triangle whose k-th row
 *            (k ≥ 2) consists of [0, (k-1)*2, (k-2)*4, …, (k -
 *            ⌊k/2⌋)*2*⌊k/2⌋]. The sequence a(n) is obtained by concatenating
 *            those rows starting with k = 2
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)	/* undefined offset; return 0 as a safe fallback */
        return 0;

    idx_t cumulative = 0;   /* number of terms processed so far */

    for (idx_t k = 2; ; ++k) {
        idx_t len = (k >> 1) + 1;   /* length of the current row:
                                      1 (for the leading 0) + floor(k/2) */
        if (cumulative + len >= n) {
            idx_t offset = n - cumulative - 1;  /* 0-based position in row */

            if (offset == 0)   /* first element of every row is 0 */
                return 0;

            seq_t kk = (seq_t)k;
            seq_t t  = (seq_t)offset; /* offset is 1 .. floor(k/2) */

            /* (k - t) * (2 * t) */
            return (kk - t) * ((seq_t)2 * t);
        }

        cumulative += len;
    }
}
