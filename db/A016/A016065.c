/*
 * A016065  a(n) = Sum_{k=0..n} k!*(k+1)!.
 * https://oeis.org/A016065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * (k+1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t fact  = 1;   /* fact = k!  (starts with 0! = 1) */

    /* term for k = 0 : 0! * 1! = 1 */
    sum += fact * fact * (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        fact  *= (seq_t)k;                 /* fact = k! */
        seq_t term = fact * fact * (seq_t)(k + 1); /* k! * (k+1)! */
        sum += term;
    }

    return sum;
}
