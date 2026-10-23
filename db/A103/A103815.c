/*
 * A103815  a(n) = -1 + Product_{k=1..n} Fibonacci(k).
 * https://oeis.org/A103815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (Product_{k=1..n} Fibonacci(k)) - 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    seq_t prod = 1;          /* running product of Fibonacci numbers */
    seq_t f_prev = 1;        /* F1 */
    seq_t f_curr = 1;        /* F2 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t f;
        if (k == 1 || k == 2)
            f = 1;              /* Fibonacci(1) = Fibonacci(2) = 1 */
        else {
            f = f_prev + f_curr;   /* compute next Fibonacci number */
            f_prev = f_curr;       /* shift for next iteration */
            f_curr = f;
        }
        prod *= f;             /* multiply the current Fibonacci number */
    }

    return prod - 1;           /* final sequence value */
}
