/*
 * A081851  Consider recurrence b(0) = (2n+1)/4, b(n) = b(0)*ceiling(b(n-1));
 *          sequence gives first integer reached (or -1 if no integer is ever
 *          reached).
 * https://oeis.org/A081851
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 26, u64 n <= 42, u128 n <= 275,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081851 – first integer reached by the recurrence
   b₀ = (2n+1)/4
   bₖ = b₀ * ⌈bₖ₋₁⌉
   a(n) = first integer in (bₖ) or –1 if none is ever reached.
   Offset: 2 (a(2) is the first listed term). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* r = 2n+1 (always odd > 1 for n ≥ 2) */
    seq_t r = (seq_t)2 * n + (seq_t)1;

    /* first ceil(b₀) = ceil(r/4) = (r+3)/4 */
    seq_t m = (r + (seq_t)3) / (seq_t)4;

    for (;;)
    {
        /* if m is divisible by 4, bₖ = r*m/4 is an integer */
        if ((m & (seq_t)3) == (seq_t)0)
            return (r * m) / (seq_t)4;

        /* next m = ceil(r*m/4) = (r*m + 3)/4 */
        seq_t prev = m;
        m = (r * m + (seq_t)3) / (seq_t)4;

        /* detect the trivial loop that occurs when r=3 (n=1, below offset) */
        if (m == prev)
            return (seq_t)-1;   /* unsigned –1 == max value */
    }
}
