/*
 * A080043  a(n)=floor((2+sqrt(7))^n).
 * https://oeis.org/A080043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((2+sqrt(7))^n) Recurrence: a(0) = 1 a(1) = 4 a(2) =
 *            21 a(3) = 100 a(n) = 4*a(n-1) + 4*a(n-2) - 4*a(n-3) - 3*a(n-4)
 *            for n ≥ 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 21;
    if (n == 3) return 100;

    /* Use only the last four values to compute further terms. */
    seq_t a0 = 1;   /* a(n-4)  */
    seq_t a1 = 4;   /* a(n-3)  */
    seq_t a2 = 21;  /* a(n-2)  */
    seq_t a3 = 100; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t ai = a3 * 4 + a2 * 4 - a1 * 4 - a0 * 3;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
