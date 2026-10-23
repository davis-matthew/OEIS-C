/*
 * A155465  a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 2; a(0) = 7, a(1) =
 *          88, a(2) = 555.
 * https://oeis.org/A155465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 2 a(0) = 7, a(1) =
 *            88, a(2) = 555
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return initial values directly */
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)88;
    if (n == 2) return (seq_t)555;

    /* Use the recurrence iteratively */
    seq_t a0 = (seq_t)7;     /* a(n-3) */
    seq_t a1 = (seq_t)88;    /* a(n-2) */
    seq_t a2 = (seq_t)555;   /* a(n-1) */
    seq_t an;                /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 7 * a2 - 7 * a1 + a0;  /* a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) */
        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2; /* when i == n, a2 == a(n) */
}
