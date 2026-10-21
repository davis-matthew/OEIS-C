/*
 * A059840  a(n) = F(n)*F(n-1) if n odd otherwise F(n)*F(n-1)-1, where F =
 *          Fibonacci numbers A000045.
 * https://oeis.org/A059840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)*F(n-1) if n is odd * a(n) = F(n)*F(n-1) - 1 if n is
 *            even * where F(k) is the Fibonacci sequence (F(0)=0, F(1)=1). *
 *            The function runs in O(n) time and uses only a constant amount
 *            of * auxiliary storage
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 – F(1) = 1, F(0) = 0. */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Compute Fibonacci numbers F(n-1) and F(n) iteratively. */
    seq_t f0 = (seq_t)0;   /* F(0) */
    seq_t f1 = (seq_t)1;   /* F(1) */
    seq_t f2;

    /* Loop from i = 2 up to i = n, inclusive. */
    for (idx_t i = 2; i <= n; ++i) {
        f2 = f1 + f0;  /* F(i) = F(i-1) + F(i-2) */
        f0 = f1;       /* shift for next step */
        f1 = f2;
    }

    /* Now f1 == F(n) and f0 == F(n-1). */
    seq_t product = f1 * f0;

    /* Apply the odd/even rule. */
    if ((n & 1) == 0) { /* n even */
        return product - (seq_t)1;
    } else {            /* n odd */
        return product;
    }
}
