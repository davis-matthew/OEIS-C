/*
 * A011580  Stirling numbers of second kind S2(31,n).
 * https://oeis.org/A011580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S(31,n) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* A static cache holds the 31 values S(31,k) for k = 0..31 */
    static seq_t cache[32];
    static int initialized = 0;

    if (!initialized) {
        /* Compute the values using the recurrence
           S(m,k) = k*S(m-1,k) + S(m-1,k-1) with S(0,0)=1, S(0,k)=0 for k>0. */
        seq_t prev[32] = {0};
        seq_t curr[32] = {0};

        prev[0] = 1; /* S(0,0) */

        for (idx_t m = 1; m <= 31; ++m) {
            curr[0] = 0;
            for (idx_t k = 1; k <= m; ++k) {
                curr[k] = (seq_t)k * prev[k] + prev[k - 1];
            }
            /* Copy current row to previous */
            for (idx_t k = 0; k <= m; ++k) {
                prev[k] = curr[k];
            }
        }

        /* Store the final row (S(31,k)) into the cache */
        for (idx_t k = 0; k <= 31; ++k) {
            cache[k] = prev[k];
        }

        initialized = 1;
    }

    if (n >= 1 && n <= 31) {
        return cache[n];
    } else {
        return 0; /* a(n) = 0 for n outside the valid range */
    }
}
