/*
 * A017997  Expansion of 1/((1-3x)*(1-7x)*(1-8x)).
 * https://oeis.org/A017997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 101*a(n-2) + 168*a(n-3) Initial values: a(0)
 *            = 1, a(1) = 18, a(2) = 223
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)223;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;        /* a(0)   */
    seq_t a1 = (seq_t)18;       /* a(1)   */
    seq_t a2 = (seq_t)223;      /* a(2)   */
    seq_t new_val;

    /* Compute terms up to a(n). */
    for (idx_t i = 3; i <= n; ++i)
    {
        new_val = a2 * 18 + a0 * 168; /* 18*a(n-1) + 168*a(n-3) */
        new_val -= a1 * 101;          /* subtract 101*a(n-2)   */

        /* Shift for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }

    return a2;
}
