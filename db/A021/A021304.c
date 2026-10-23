/*
 * A021304  Expansion of 1/((1-x)(1-2x)(1-9x)(1-12x)).
 * https://oeis.org/A021304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021304 – Expansion of 1/((1-x)(1-2x)(1-9x)(1-12x))
   Recurrence (for n > 3):
     a(0) = 1,
     a(1) = 24,
     a(2) = 403,
     a(3) = 5886,
     a(n) = 24*a(n-1) - 173*a(n-2) + 366*a(n-3) - 216*a(n-4).    */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)403;
    if (n == 3) return (seq_t)5886;

    /* Iteratively apply the linear recurrence. */
    seq_t a0 = (seq_t)1;      /* a(n-4)   */
    seq_t a1 = (seq_t)24;     /* a(n-3)   */
    seq_t a2 = (seq_t)403;    /* a(n-2)   */
    seq_t a3 = (seq_t)5886;   /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* next = 24*a3 - 173*a2 + 366*a1 - 216*a0 */
        seq_t next = (seq_t)24 * a3
                   - (seq_t)173 * a2
                   + (seq_t)366 * a1
                   - (seq_t)216 * a0;

        /* shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
