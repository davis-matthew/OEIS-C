/*
 * A163613  a(n) = ((1 + 3*sqrt(2))*(2 + sqrt(2))^n + (1 - 3*sqrt(2))*(2 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) (n > 1); a(0) = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* Use the recurrence iteratively. */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)8;      /* a(1) */
    seq_t ai = 0;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)(4 * a1) - (seq_t)(2 * a0);  /* a(i) = 4*a(i-1) - 2*a(i-2) */
        a0 = a1;
        a1 = ai;
    }

    return ai;
}
