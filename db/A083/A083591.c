/*
 * A083591  Inverse binomial transform of A083589.
 * https://oeis.org/A083591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + 6*a(n-2) + 14*a(n-3) + 12*a(n-4) with initial
 *            values: a(0)=1, a(1)=3, a(2)=9, a(3)=27, a(4)=82
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)82;

    /* rolling variables for the last four terms:
       a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1)  */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)9;   /* a(2) */
    seq_t a3 = (seq_t)27;  /* a(3) */
    seq_t a4 = (seq_t)82;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = -a(i-1) + 6*a(i-2) + 14*a(i-3) + 12*a(i-4) */
        seq_t new_term = 12 * a1 + 14 * a2 + 6 * a3;
        new_term -= a4;          /* subtraction is safe: result is non‑negative */

        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_term;
    }

    return a4;
}
