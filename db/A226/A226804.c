/*
 * A226804  Expansion of 1/((1-3x)(1-9x)(1-27x)(1-81x)).
 * https://oeis.org/A226804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Expansion of 1/((1-3x)(1-9x)(1-27x)(1-81x))
   Recurrence: a(n) = 120*a(n-1) - 3510*a(n-2) + 29160*a(n-3) - 59049*a(n-4)
   Initial values: a(0) = 1, a(1) = 120, a(2) = 10890, a(3) = 914760 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)120;
    if (n == 2) return (seq_t)10890;
    if (n == 3) return (seq_t)914760;

    /* Use the four-term linear recurrence for n ≥ 4 */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)120;        /* a(n-3) */
    seq_t a2 = (seq_t)10890;      /* a(n-2) */
    seq_t a3 = (seq_t)914760;     /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        /* New value: 120*a3 - 3510*a2 + 29160*a1 - 59049*a0 */
        seq_t new_val = (seq_t)120 * a3;
        new_val -= (seq_t)3510 * a2;
        new_val += (seq_t)29160 * a1;
        new_val -= (seq_t)59049 * a0;

        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3;
}
