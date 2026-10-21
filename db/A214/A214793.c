/*
 * A214793  a(n)= smallest k>=2 such that n divides Fibonacci(k-1)+13, or 0 if
 *          no such k exists.
 * https://oeis.org/A214793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 2 such that n divides Fibonacci(k−1)+13; 0
 *            if no such k
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* protect against division by zero */

    /* The Pisano period of n is at most 6·n, so checking up to that gives a guaranteed
       complete search for the first qualifying k. */
    idx_t limit = 6 * n;

    idx_t f_prev = 0;                 /* Fib(0) mod n */
    idx_t f_cur  = 1 % n;             /* Fib(1) mod n */

    for (idx_t j = 1; j <= limit; ++j) {
        /* k = j+1 corresponds to Fib(j) */
        if ((f_cur + 13) % n == 0) {
            return (seq_t)(j + 1);
        }

        /* Next Fibonacci number modulo n */
        idx_t f_next = (f_prev + f_cur) % n;
        f_prev = f_cur;
        f_cur  = f_next;
    }

    return 0;                          /* no such k exists within one Pisano period */
}
