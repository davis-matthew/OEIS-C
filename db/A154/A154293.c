/*
 * A154293  Integers of the form t/6, where t is a triangular number
 *          (A000217).
 * https://oeis.org/A154293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154293: Integers of the form t/6 where t is triangular.
   Formula used: a(n) = (f(n-1)^2 - 1) / 48;   f(k) = 8*(k - floor(k/4)) + (-1)^k.
   Offsets: a(1)=0.                                                     */

seq_t a(idx_t n)
{
    /* Handle the first term directly (not strictly necessary). */
    if (n <= 1)
        return (seq_t)0;

    /* Use the offset 0 formula with k = n-1. */
    idx_t k = n - 1;

    /* diff = k - floor(k/4)   (all unsigned). */
    seq_t diff = (seq_t)k - (seq_t)(k / 4);

    /* f(k) = 8*diff ± 1 depending on the parity of k. */
    seq_t f = (k & 1) ? (8 * diff - 1) : (8 * diff + 1);

    /* a(n) = (f^2 - 1) / 48.  All operations are integer. */
    seq_t square = f * f;
    seq_t result = (square - 1) / 48;

    return result;
}
