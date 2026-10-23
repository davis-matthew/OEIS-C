/*
 * A157826  a(n) = 103680000*n^2 - 194428800*n + 91152001.
 * https://oeis.org/A157826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 103680000*n^2 - 194428800*n + 91152001
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A157826 for index n (offset 1). */
/* The calculation is done with unsigned arithmetic; intermediate terms
 * are arranged to avoid underflow:  term = 103680000*n^2 + 91152001
 *  then s = term – 194428800*n. */
seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* Compute 103680000*n^2 + 91152001 safely */
    seq_t term = (seq_t)103680000 * nn * nn + 91152001;
    /* Subtract 194428800*n; term is always >= this value for n >= 1 */
    return term - (seq_t)194428800 * nn;
}
