/*
 * A036546  a(n) = T(5,n), array T given by A048471.
 * https://oeis.org/A036546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*3^n - 15
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 3*a(n-1) + 30 */

seq_t a(idx_t n)
{
    /* Start with the initial value a(0) = 1 */
    seq_t val = (seq_t)1;

    /* Use the simple linear recurrence to compute a(n) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)3 + (seq_t)30;
    }

    return val;
}
