/*
 * A017362  a(n) = (10*n + 7)^10.
 * https://oeis.org/A017362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (10*n + 7) in seq_t arithmetic. */
    seq_t base = (seq_t)(10 * (seq_t)n + 7);

    /* Raise the base to the 10th power by repeated multiplication. */
    seq_t v = 1;
    for (idx_t i = 0; i < 10; ++i) {
        v *= base;
    }
    return v;
}
