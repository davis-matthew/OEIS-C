/*
 * A174662  Partial sums of A003149.
 * https://oeis.org/A174662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} Sum_{k=0..i} k! * (i-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val  = (seq_t)1;   /* a(0) */
    seq_t b_prev = (seq_t)1;   /* b(0) = 0! * 0! = 1 */
    seq_t fact   = (seq_t)1;   /* 0! */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;                   /* i!  */

        seq_t temp = b_prev * (seq_t)(i + 1); /* (n+1) * b(n-1) */
        temp /= (seq_t)2;                  /* divide by 2, exact */
        temp += fact;                      /* b(n) */

        a_val += temp;                     /* a(n) = a(n-1) + b(n) */
        b_prev = temp;                     /* update b for next iteration */
    }

    return a_val;
}
