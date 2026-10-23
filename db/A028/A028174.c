/*
 * A028174  Expansion of 1/((1-5*x)*(1-6*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A028174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*10^(n+2)-3*9^(n+2)+2*6^(n+2)-5^(n+2))/4. Recurrence:
 *            a(n) = 30*a(n-1)-329*a(n-2)+1560*a(n-3)-2700*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)571;
    if (n == 3) return (seq_t)8820;

    /* Variables for the last four computed terms */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)30;     /* a(n-3) */
    seq_t a2 = (seq_t)571;    /* a(n-2) */
    seq_t a3 = (seq_t)8820;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)30 * a3
                     - (seq_t)329 * a2
                     + (seq_t)1560 * a1
                     - (seq_t)2700 * a0;

        /* Shift the window of last four terms */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3; /* a(n) */
}
