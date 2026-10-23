/*
 * A126509  Number of base 14 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base-14 n-digit strings (digits 0–13) with *
 *            |adjacent digits| ≤ 4. Computed by dynamic programming
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;

    const int B = 14;              /* number of digits in base 14 */
    seq_t prev[B], cur[B];

    /* initialise for length 1: each digit occurs once */
    for (int d = 0; d < B; ++d) prev[d] = 1;

    /* iterate for positions 2 to n */
    for (idx_t pos = 1; pos < n; ++pos) {
        for (int d = 0; d < B; ++d) {
            seq_t sum = 0;
            int lo = d - 4; if (lo < 0) lo = 0;
            int hi = d + 4; if (hi > B - 1) hi = B - 1;
            for (int k = lo; k <= hi; ++k) sum += prev[k];
            cur[d] = sum;
        }
        /* copy current to previous for next iteration */
        for (int d = 0; d < B; ++d) prev[d] = cur[d];
    }

    /* total count is the sum over all ending digits */
    seq_t total = 0;
    for (int d = 0; d < B; ++d) total += prev[d];
    return total;
}
