/*
 * A020594  Expansion of 1/((1-6x)(1-8x)(1-12x)).
 * https://oeis.org/A020594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 216*a(n-2) + 576*a(n-3) with initial values
 *            a(0)=1, a(1)=26, a(2)=460
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)460;

    /* iterate the recurrence up to n */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)26;  /* a(n-2) */
    seq_t a2 = (seq_t)460; /* a(n-1) */
    seq_t a3;              /* current term */

    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = (seq_t)26 * a2 - (seq_t)216 * a1 + (seq_t)576 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
