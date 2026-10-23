/*
 * A192255  1-sequence of reduction of (n^2) by x^2 -> x+1.
 * https://oeis.org/A192255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} i^2 * Fib(i-1) (Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1 – guard for safety */
        return 0;

    seq_t sum  = 0;            /* running total */
    seq_t fib0 = 0;            /* F_{k-1}  */
    seq_t fib1 = 1;            /* F_k      */

    /* sum_{i=2..n} i^2 * F_{i-1} ; the i=1 term is 0 */
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            seq_t term = fib1 * (seq_t)i * (seq_t)i;
            sum += term;

            /* advance Fibonacci */
            seq_t fib_next = fib0 + fib1;
            fib0 = fib1;
            fib1 = fib_next;
        }
    }
    return sum;
}
