/*
 * A179091  Variant of Sylvester's sequence: a(n + 1) = a(n)^2 - a(n) + 1,
 *          with a(1) = 15.
 * https://oeis.org/A179091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 - a(n) + 1, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* seq_t may be an unsigned 8/16/32/64/128‐bit integer or an
       arbitrary‑precision integer class that supports +, -, *,
       and has a default constructor.  All operations are performed
       in that type.  */
    seq_t val = (seq_t)15;          /* a(1) */
    /* If n is 1 we are already done.  For n > 1 iterate the recurrence. */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t t = val;
        val = t * t - t + (seq_t)1; /* a(i+1) = a(i)^2 - a(i) + 1 */
    }
    return val;
}
