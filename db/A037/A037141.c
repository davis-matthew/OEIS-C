/*
 * A037141  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          for k >= -5, with F(-n)=(-1)^(n+1)*F(n);.
 * https://oeis.org/A037141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Formula: a(n) = F(n-1) + (4 + 3*n)
 * where F(k) is the Fibonacci sequence extended to negative indices by
 * F(-k) = (-1)^(k+1) * F(k).
 * For n=0, F(-1) = 1, so a(0) = 1 + 4 + 0 = 5.
 */
seq_t a(idx_t n)
{
    seq_t fib; /* F(n-1) or F(-1) when n==0 */

    if (n == 0) {
        /* F(-1) = 1 by the extended rule */
        fib = 1;
    } else {
        /* compute F(n-1) by iteration */
        seq_t f0 = 0, f1 = 1;
        if (n == 1) {
            fib = 0;            /* F(0) = 0 */
        } else {
            for (idx_t i = 2; i <= n - 1; ++i) {
                seq_t t = f0 + f1;
                f0 = f1;
                f1 = t;
            }
            fib = f1; /* f1 holds F(n-1) after the loop */
        }
    }

    /* a(n) = fib + 4 + 3*n, using seq_t arithmetic for all terms */
    seq_t term = fib + ((seq_t)4 + ((seq_t)n * (seq_t)3));
    return term;
}
