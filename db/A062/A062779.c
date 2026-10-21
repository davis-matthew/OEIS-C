/*
 * A062779  a(n) = 2*n*(2*n)!.
 * https://oeis.org/A062779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(2*n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 0 using only integer arithmetic.
 * For n = 0 the result is 0, otherwise the factorial of (2*n)
 * is built iteratively and multiplied by 2*n.  All operations
 * are performed in the type seq_t, which may be an unsigned
 * integer or an arbitrary‑precision integer class. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t fact = 1;                 /* fact = (2*n)!  */
    idx_t twice = 2 * n;
    for (idx_t i = 2; i <= twice; ++i)
        fact *= (seq_t)i;

    /* a(n) = 2*n * (2*n)! = twice * fact */
    return fact * (seq_t)twice;
}
