/*
 * A045754  7-fold factorials: a(n) = Product_{k=0..n-1} (7*k+1).
 * https://oeis.org/A045754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1} (7*k+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n-th 7‑fold factorial. */
seq_t a(idx_t n) {
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = ((seq_t)7 * (seq_t)k) + 1;
        res *= term;
    }
    return res;
}
