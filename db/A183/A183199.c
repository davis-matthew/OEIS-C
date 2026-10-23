/*
 * A183199  Least integer k such that Floor(k*f(n+1))>k*f(n), where
 *          f(n)=(n^2)/(1+n^2).
 * https://oeis.org/A183199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^2 + 2
 * (Least integer k such that ⌊k*f(n+1)⌋ > k*f(n),
 *  with f(n) = n^2/(1+n^2). This simplifies to k = n^2 + 2.)
 */
seq_t a(idx_t n)
{
    /* Guarantee using seq_t arithmetic on n. */
    return (seq_t)n * (seq_t)n + 2;
}
