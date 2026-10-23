/*
 * A131234  Starts with 1, then n appears Fibonacci(n-1) times.
 * https://oeis.org/A131234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that Fibonacci(k + 1) ≥ n (where
 *            Fibonacci(1)=1, Fibonacci(2)=1, Fibonacci(3)=2, …)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* not required by OEIS, but safe guard */
        return 0;

    seq_t sn = (seq_t)n;        /* convert index to seq_t */
    seq_t f0  = (seq_t)0;       /* Fibonacci(i‑1)   */
    seq_t f1  = (seq_t)1;       /* Fibonacci(i)     */

    for (idx_t k = 1; ; ++k) {
        seq_t f_next = f0 + f1;            /* Fibonacci(k+1)  */
        if (f_next >= sn)
            return (seq_t)k;               /* a(n) = k         */
        f0 = f1;
        f1 = f_next;
    }
}
