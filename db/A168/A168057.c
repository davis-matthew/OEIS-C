/*
 * A168057  Expansion of (1+8x^2+8x^3)/((1-2x)^2*(1+2x+4x^2)).
 * https://oeis.org/A168057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=12, a(3)=40, a(n) = 2*a(n-1) + 8*a(n-3) -
 *            16*a(n-4) for n >= 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A168057. */
seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)12, (seq_t)40 };

    if (n < 4)                /* base cases */
        return init[n];

    /* Keep the last four values needed for the recurrence. */
    seq_t prev[4] = { init[0], init[1], init[2], init[3] };

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t val = ((seq_t)2) * prev[3] + ((seq_t)8) * prev[1] - ((seq_t)16) * prev[0];

        /* Shift the window: prev[0] <- a(i-3), prev[1] <- a(i-2),
           prev[2] <- a(i-1), prev[3] <- a(i).                     */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = val;
    }
    return prev[3];
}
