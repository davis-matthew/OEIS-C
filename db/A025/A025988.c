/*
 * A025988  Expansion of 1/((1-2*x)*(1-5*x)*(1-6*x)*(1-9*x)).
 * https://oeis.org/A025988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(n+1)-2^(n+1))/3 + 15*a(n-1) - 54*a(n-2); a(0) = 1,
 *            a(1) = 22
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;

    /* a0 = a(n-2), a1 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)22;  /* a(1) */

    /* pow2_nplus1 holds 2^(i+1); pow5_nplus1 holds 5^(i+1) */
    seq_t pow2_nplus1 = (seq_t)4; /* 2^2, the value needed for n = 1 */
    seq_t pow5_nplus1 = (seq_t)25; /* 5^2, the value needed for n = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* advance the powers for the next n */
        pow2_nplus1 *= (seq_t)2; /* now 2^(i+1) */
        pow5_nplus1 *= (seq_t)5; /* now 5^(i+1) */

        /* term = (5^(i+1) - 2^(i+1)) / 3 */
        seq_t term = (pow5_nplus1 - pow2_nplus1) / (seq_t)3;

        /* recurrence: a(i) = term + 15*a(i-1) - 54*a(i-2) */
        seq_t ai = term + (seq_t)15 * a1 - (seq_t)54 * a0;

        /* shift for next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1; /* a(n) is stored in a1 after the loop */
}
