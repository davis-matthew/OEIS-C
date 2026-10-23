/*
 * A008876  3x+1 sequence starting at 81.
 * https://oeis.org/A008876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008876: Collatz sequence starting at 81.
   a(0) = 81; for n > 0, a(n) = a(n-1)/2 if a(n-1) is even,
   otherwise a(n) = 3*a(n-1) + 1.  For n > 22 the sequence enters
   the 1‑4‑2 cycle, so the rule applies naturally. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)81;               /* starting value a(0) */
    idx_t i;
    for (i = 0; i < n; ++i)
    {
        /* Branchless step: if x is odd, multiply by 3 and add 1,
           otherwise divide by 2. */
        if ((x & (seq_t)1) == 0)       /* even */
            x = x / (seq_t)2;
        else
            x = (seq_t)3 * x + (seq_t)1;
    }
    return x;
}
