/*
 * A021503  Expansion of 1/((1-x)(1-3x)(1-6x)(1-8x)).
 * https://oeis.org/A021503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 18*a(n-1) - 107*a(n-2) + 234*a(n-3) - 144*a(n-4)
 * Initial values: a(0) = 1, a(1) = 18, a(2) = 217, a(3) = 2214.
 * (Derived from the OEIS formula a(n) = (3*8^(n+3)-7*6^(n+3)+7*3^(n+3)-3)/210.) */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)217;
    if (n == 3) return (seq_t)2214;

    /* Use the 4‑term linear recurrence for n ≥ 4. */
    seq_t a0 = (seq_t)1;          /* a(n-4)   */
    seq_t a1 = (seq_t)18;         /* a(n-3)   */
    seq_t a2 = (seq_t)217;        /* a(n-2)   */
    seq_t a3 = (seq_t)2214;       /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)18 * a3
           - (seq_t)107 * a2
           + (seq_t)234 * a1
           - (seq_t)144 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;   /* a(n) is stored in a3 after the loop. */
}
