/*
 * A022354  Fibonacci sequence beginning 0, 20.
 * https://oeis.org/A022354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20 * F(n), where F(0)=0, F(1)=1, F(k)=F(k-1)+F(k-2)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A022354: 20 times the Fibonacci number. */
seq_t a(idx_t n)
{
    /* Handle the lower indices explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)20;

    /* Iterate to compute F(n) using only seq_t arithmetic. */
    seq_t f0 = (seq_t)0;   /* F(0) */
    seq_t f1 = (seq_t)1;   /* F(1) */
    seq_t fn;

    for (idx_t i = 2; i <= n; ++i)
    {
        fn = f0 + f1;  /* F(i) = F(i-2) + F(i-1) */
        f0 = f1;
        f1 = fn;
    }

    /* Return 20 * F(n) */
    return f1 * (seq_t)20;
}
