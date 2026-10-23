/*
 * A161022  Collatz (or 3x+1) trajectory starting at 10087.
 * https://oeis.org/A161022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 223, u64 n <= 223, u128 n <= 223,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Collatz (3x+1) trajectory starting at 10087.
   a(0) = 10087,  a(n+1) = a(n)/2 if a(n) even,
   a(n+1) = a(n)*3 + 1 if a(n) odd. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)10087;          // a(0)
    for (idx_t i = 0; i < n; ++i)
    {
        /* test evenness with a bit check – works for all seq_t types */
        if ((x & 1) == 0)
            x = x >> 1;            // even: divide by 2
        else
            x = x * 3 + 1;         // odd: 3*x + 1
    }
    return x;
}
