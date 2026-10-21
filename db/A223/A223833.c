/*
 * A223833  Number of n X 3 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 9*n^2 + 35*n)/6 - 1 (for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    (derived from the empirical cubic formula in the OEIS entry) */

seq_t a(idx_t n)
{
    /* For safety, return 0 for the off‑shelf index 0. */
    if (n == 0)
        return (seq_t)0;

    /* Promote index to the sequence type before any arithmetic. */
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;            /* n^2   */
    seq_t n3    = n2 * n_seq;               /* n^3   */

    /* Compute the numerator: 4*n^3 + 9*n^2 + 35*n   (fits in seq_t for n≈10‑4) */
    seq_t num   = 4 * n3 + 9 * n2 + 35 * n_seq;

    /* Divide by 6 (exact division) and subtract 1. */
    return num / (seq_t)6 - (seq_t)1;
}
