/*
 * A080145  a(n) = Sum_{m=1..n} Sum_{i=1..m} F(i)*F(i+1) where F(n)=Fibonacci
 *          numbers A000045.
 * https://oeis.org/A080145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n+1)*F(n+2) - floor((n+2)/2) (where F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Fibonacci numbers iteratively up to n+2; time O(n). */

seq_t a(idx_t n)
{
    /* Fibonacci variables: f_prev = F(k-1), f_curr = F(k) */
    seq_t f_prev = 0;      /* F(0) */
    seq_t f_curr = 1;      /* F(1) */

    idx_t upto = n + 2;    /* compute up to F(n+2) */

    /* iterate from k = 2 to upto */
    for (idx_t k = 2; k <= upto; ++k) {
        seq_t temp = f_prev + f_curr;  /* F(k) = F(k-1)+F(k-2) */
        f_prev = f_curr;               /* shift */
        f_curr = temp;
    }

    /* After the loop:
       f_prev = F(n+1), f_curr = F(n+2) */
    seq_t prod = f_prev * f_curr;

    /* Subtract floor((n+2)/2) */
    seq_t sub = (seq_t)((n + 2) / 2);

    return prod - sub;
}
