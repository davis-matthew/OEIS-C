/*
 * A217471  Partial sum of fifth power of the even-indexed Fibonacci numbers.
 * https://oeis.org/A217471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} F(2k)^5 where F(m) is the m-th Fibonacci
 *            number, F(0)=0, F(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n == 0 the result is 0, handled by the loop below. */
    seq_t sum = 0;

    /* Fibonacci iterator: f_prev = F(pos), f_curr = F(pos+1). */
    seq_t f_prev = (seq_t)0;   /* F(0) */
    seq_t f_curr = (seq_t)1;   /* F(1) */

    idx_t pos = 0;                       /* current index of f_prev */
    idx_t maxpos = n << 1; /* 2*n, the highest index needed */

    while (pos <= maxpos) {
        /* Process even indices only. */
        if ((pos & 1) == 0) {
            seq_t f2 = f_prev * f_prev;
            seq_t f4 = f2 * f2;
            seq_t f5 = f4 * f_prev;
            sum += f5;
        }

        /* Advance Fibonacci numbers: compute next Fibonacci. */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        pos++;
    }

    return sum;
}
