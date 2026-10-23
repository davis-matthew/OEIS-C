/*
 * A179870  a(n) = ((n-1)! * (n+1)!) ^ 2.
 * https://oeis.org/A179870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)! * (n+1)!) ^ 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute factorials up to n+1.
       fact_n_minus1 holds (n-1)!   (for n==1 it stays 1)
       fact_n_plus1  holds (n+1)!   (for n==1 it becomes 2)    */
    seq_t fact = 1;
    seq_t fact_n_minus1 = 1;
    seq_t fact_n_plus1;

    /* n >= 1, so the loop will always run at least once (i=2). */
    for (idx_t i = 2; i <= n + 1; ++i) {
        fact = fact * (seq_t)i;        /* fact = i! */
        if (i == n - 1) {
            fact_n_minus1 = fact;       /* (n-1)! */
        }
        if (i == n + 1) {
            fact_n_plus1 = fact;        /* (n+1)! */
        }
    }

    /* For n == 1, fact_n_minus1 stays 1 and fact_n_plus1 is 2. */
    seq_t prod = fact_n_minus1 * fact_n_plus1;   /* (n-1)! * (n+1)! */
    prod = prod * prod;                         /* square */
    return prod;
}
