/*
 * A013979  Expansion of 1/(1 - x^2 - x^3 - x^4) = 1/((1 + x)*(1 - x - x^3)).
 * https://oeis.org/A013979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4) for n ≥ 4 Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1 This implements the recurrence for the
 *            number of compositions of n into the parts 2, 3 and 4
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small cases directly. */
    if (n == 0u) return 1;
    if (n == 1u) return 0;
    if (n == 2u) return 1;
    if (n == 3u) return 1;

    /* Recurrence cache: keep the last five values needed for the next step.
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) at each iteration. */
    seq_t a0 = 1;
    seq_t a1 = 0;
    seq_t a2 = 1;
    seq_t a3 = 1;
    seq_t a4 = 0;             /* temporary holder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a4 = a0 + a1 + a2;    /* a(i) = a(i-2)+a(i-3)+a(i-4) */
        a0 = a1;              /* shift window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;                /* a(n) is now stored in a3 */
}
