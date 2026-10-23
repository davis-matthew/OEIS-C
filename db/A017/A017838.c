/*
 * A017838  Expansion of 1/(1-x^5-x^6-x^7).
 * https://oeis.org/A017838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑5) + a(n‑6) + a(n‑7), a(0) = 1, a(1) = 0, a(2) = 0,
 *            a(3) = 0, a(4) = 0, a(5) = 1, a(6) = 1, a(7) = 1 (number of
 *            compositions of n into parts 5, 6 and 7)
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 129, u64 n <= 249,
 *            u128 n <= 489, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a small ring buffer of the last 8 values */
    seq_t last[8];
    last[0] = (seq_t)1;
    last[1] = last[2] = last[3] = last[4] = (seq_t)0;
    last[5] = last[6] = last[7] = (seq_t)1;

    /* Direct return for the first 8 indices */
    if (n <= 7u)
        return last[n];

    /* Iterate from 8 to n, using a circular buffer */
    for (idx_t i = 8; i <= n; ++i) {
        idx_t idx1 = (i - 5) & 7; /* i-5 mod 8 */
        idx_t idx2 = (i - 6) & 7; /* i-6 mod 8 */
        idx_t idx3 = (i - 7) & 7; /* i-7 mod 8 */
        seq_t val = last[idx1] + last[idx2] + last[idx3];
        last[i & 7] = val;       /* store at position i mod 8 */
    }
    return last[n & 7];
}
