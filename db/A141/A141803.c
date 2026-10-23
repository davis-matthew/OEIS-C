/*
 * A141803  Triangle read by rows derived from generalized Thue-Morse
 *          sequences.
 * https://oeis.org/A141803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the smallest integer such that r*(r+1)/2 ≥ n. Let k = n
 *            – r*(r−1)/2 (1 ≤ k ≤ r). Then a(n) = sum of the digits of k in
 *            base (r−k+2), modulo (r−k+2). This corresponds to the table
 *            entry A(m,ncol) with m = r−k+1, base = m+1, and column index
 *            ncol = k
 * Range:     u8 n <= 8001, u16 n <= 8001, u32 n <= 8001, u64 n <= 8001,
 *            u128 n <= 8001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the row r that contains the n‑th term. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Position k (1‑based) within that row. */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* Translate to array indices: m = r−k+1, base = m+1. */
    idx_t m = r - k + 1;
    idx_t base = m + 1;

    /* Sum the digits of k in the given base. */
    seq_t sum = 0;
    idx_t tmp = k;
    while (tmp) {
        sum += tmp % base;
        tmp /= base;
    }

    /* Return the value modulo the base. */
    return (seq_t)(sum % base);
}
