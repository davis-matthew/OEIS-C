/*
 * A045624  Row sums of convolution triangle A030526.
 * https://oeis.org/A045624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 17*a(n-1) - 102*a(n-2) + 272*a(n-3) - 272*a(n-4); Initial
 *            values: a(1)=1, a(2)=11, a(3)=101, a(4)=851
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)101;
    if (n == 4) return (seq_t)851;

    /* Working variables for the last four values:
       a1 = a(k-4), a2 = a(k-3), a3 = a(k-2), a4 = a(k-1)   */
    seq_t a1 = (seq_t)1;     /* a(1) */
    seq_t a2 = (seq_t)11;    /* a(2) */
    seq_t a3 = (seq_t)101;   /* a(3) */
    seq_t a4 = (seq_t)851;   /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute a(i) from the recurrence, ordering the
           operations so that the intermediate sum stays
           non‑negative for unsigned seq_t. */
        cur = a4 * (seq_t)17;        /* 17*a(i-1) */
        cur += a2 * (seq_t)272;      /* +272*a(i-3) */
        cur -= a3 * (seq_t)102;      /* -102*a(i-2) */
        cur -= a1 * (seq_t)272;      /* -272*a(i-4) */

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}
