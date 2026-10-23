/*
 * A048577  Pisot sequence L(3,4).
 * https://oeis.org/A048577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+3) + 1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci number F(n+3) iteratively and return F(n+3)+1.  */
    idx_t nplus3 = n + (idx_t)3;

    if (nplus3 == 0) {          /* F0 = 0  */
        return (seq_t)1;        /* 0 + 1  */
    }
    if (nplus3 == 1) {          /* F1 = 1  */
        return (seq_t)2;        /* 1 + 1  */
    }

    seq_t f_prev = 0;           /* F0          */
    seq_t f_curr = 1;           /* F1          */
    for (idx_t i = 2; i <= nplus3; ++i) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    /* f_curr now holds F(n+3) */
    return f_curr + (seq_t)1;
}
