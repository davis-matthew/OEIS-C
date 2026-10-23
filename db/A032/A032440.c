/*
 * A032440  Sum_{i=0..3} binomial(Fibonacci(n),i).
 * https://oeis.org/A032440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^3 binomial(Fibonacci(n), i) (with Fibonacci(0)=0,
 *            Fibonacci(1)=1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) with F(0)=0, F(1)=1 using an iterative loop. */
    seq_t f0 = 0;
    seq_t f1 = 1;
    seq_t fn;
    if (n == 0) {
        fn = f0;
    } else if (n == 1) {
        fn = f1;
    } else {
        for (idx_t i = 2; i <= n; ++i) {
            seq_t temp = f0 + f1;
            f0 = f1;
            f1 = temp;
        }
        fn = f1;
    }

    /* Sum the first four binomial coefficients C(fn, i). */
    seq_t sum = 1;                 /* C(fn,0) = 1 */
    if (fn >= 1) sum += fn;        /* C(fn,1)  */
    if (fn >= 2) sum += fn * (fn - 1) / 2;             /* C(fn,2) */
    if (fn >= 3) sum += fn * (fn - 1) * (fn - 2) / 6;   /* C(fn,3) */

    return sum;
}
