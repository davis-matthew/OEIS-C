/*
 * A126511  Number of base 16 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A126511: number of base‑16 n‑digit numbers whose
 * adjacent digits differ by at most four.  The count is derived by a
 * simple dynamic‑programming scan over all 16 possible last digits.
 */
seq_t a(idx_t n)
{
    /* base case: the empty string */
    if (n == 0)
        return (seq_t)1;

    /* dp_prev[d] – number of sequences of the current length ending in digit d */
    seq_t dp_prev[16];
    seq_t dp_curr[16];

    /* for length 1 each digit can appear exactly once */
    for (idx_t d = 0; d < 16; ++d)
        dp_prev[d] = (seq_t)1;

    /* iterate from length 1 up to length n-1 */
    for (idx_t len = 1; len < n; ++len) {
        for (idx_t d = 0; d < 16; ++d) {
            seq_t sum = (seq_t)0;
            /* add contributions from all preceding last digits whose
             * differences are at most four */
            for (idx_t prev = 0; prev < 16; ++prev) {
                if (prev == d ||
                    (prev < d && d - prev <= 4) ||
                    (prev > d && prev - d <= 4))
                    sum = sum + dp_prev[prev];
            }
            dp_curr[d] = sum;
        }
        /* advance to the next length */
        for (idx_t d = 0; d < 16; ++d)
            dp_prev[d] = dp_curr[d];
    }

    /* sum all states to obtain the total number of n‑digit sequences */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < 16; ++d)
        total = total + dp_prev[d];

    return total;
}
