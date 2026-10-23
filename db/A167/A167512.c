/*
 * A167512  Number of Simple Alternating Transit (SAT) mazes with exactly two
 *          extreme values.
 * https://oeis.org/A167512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = sum_{k=1..n} d(k), where * d(0) = 0, d(1) = 0, d(2) = 0,
 *            * for even k ≥ 4 : d(k) = F(k-2) + d(k-2), * for odd k ≥ 3 :
 *            d(k) = d(k-1), * and F(m) are Fibonacci numbers with F(0)=0,
 *            F(1)=1
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 0;           /* cumulative sum a(n) */
    seq_t diff_even_prev = 0;  /* d(k) for the previous even k  */
    seq_t fib_prev = 0;        /* F(i-2) for current i          */
    seq_t fib_cur  = 1;        /* F(i-1) for current i          */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t diff;

        if (i == 1) {          /* d(1) */
            diff = 0;
        } else if (i == 2) {   /* d(2) */
            diff = 0;
            diff_even_prev = diff;
        } else {
            if ((i & 1) == 0) {            /* even i */
                diff = fib_prev + diff_even_prev;
                diff_even_prev = diff;
            } else {                       /* odd i */
                diff = diff_even_prev;
            }
        }

        a_val += diff;    /* update a(n) */

        if (i >= 2) {     /* update Fibonacci numbers for next iteration */
            seq_t fib_next = fib_prev + fib_cur; /* F(i) */
            fib_prev = fib_cur;                 /* shift to F(i-1) */
            fib_cur  = fib_next;                /* shift to F(i)   */
        }
    }

    return a_val;
}
