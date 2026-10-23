/*
 * A103771  Expansion of 1/(1-4x-4x^2-4x^3).
 * https://oeis.org/A103771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)+4*a(n-3), with a(0)=1, a(1)=4, a(2)=20
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)20;

    /* Iterate from 3 up to n using the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)20;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        /* ai = 4*a2 + 4*a1 + 4*a0 */
        ai = (seq_t)4 * a2;
        ai += (seq_t)4 * a1;
        ai += (seq_t)4 * a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return ai;
}
