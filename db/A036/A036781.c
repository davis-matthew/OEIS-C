/*
 * A036781  a(n) = n + Sum_{k=0..n} k!.
 * https://oeis.org/A036781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=0}^{n} k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 1;          /* 0! = 1 */
    seq_t fact  = 1;          /* current factorial value */
    for (idx_t k = 1; k <= n; ++k) {
        fact = fact * (seq_t)k;   /* k! = (k-1)! * k */
        sum  = sum  + fact;      /* accumulate Σ k! */
    }
    return (seq_t)n + sum;        /* a(n) = n + Σ k! */
}
