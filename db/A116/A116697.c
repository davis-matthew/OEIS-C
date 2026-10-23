/*
 * A116697  a(n) = cos(n*Pi/2)-F(n)*(-1)^n, where F(n) = Fibonacci(n).
 * https://oeis.org/A116697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign
 * Formula:   a(n) = cos(n*Pi/2) - F(n)*(-1)^n
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F(n) is the Fibonacci sequence with F(0)=0, F(1)=1. */
seq_t a(idx_t n)
{
    /* compute the n-th Fibonacci number iteratively */
    seq_t fib_n;
    if (n == 0) {
        fib_n = 0;
    } else if (n == 1) {
        fib_n = 1;
    } else {
        seq_t f0 = 0;
        seq_t f1 = 1;
        seq_t temp;
        for (idx_t i = 2; i <= n; ++i) {
            temp = f0 + f1;
            f0 = f1;
            f1 = temp;
        }
        fib_n = f1;
    }

    /* cos(n*Pi/2) values for n mod 4 */
    seq_t cos_term;
    switch (n & 3ULL) {
        case 0: cos_term = 1; break;
        case 1: cos_term = 0; break;
        case 2: cos_term = -1; break;
        case 3: cos_term = 0; break;
    }

    /* (-1)^n sign */
    if ((n & 1) == 0) {
        /* even n: subtract Fibonacci */
        return cos_term - fib_n;
    } else {
        /* odd n: add Fibonacci */
        return cos_term + fib_n;
    }
}
