/*
 * A025996  Expansion of 1/((1-2*x)*(1-5*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A025996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 26*a(n-1) - 227*a(n-2) + 778*a(n-3) - 840*a(n-4); initial
 *            values: a(0)=1, a(1)=26, a(2)=449, a(3)=6550
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)449;
    if (n == 3) return (seq_t)6550;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)26;  /* a(1) */
    seq_t a2 = (seq_t)449; /* a(2) */
    seq_t a3 = (seq_t)6550;/* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)26 * a3
                     - (seq_t)227 * a2
                     + (seq_t)778 * a1
                     - (seq_t)840 * a0;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
