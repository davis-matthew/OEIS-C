/*
 * A097345  Numerators of the partial sums of the binomial transform of
 *          1/(n+1).
 * https://oeis.org/A097345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction of S(n) = Σ_{k=0}^n
 *            (2^{k+1} - 1)/(k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t pow = 1;            /* 2^k for current k (k starts at 0).  */
    seq_t sum_num = 0;        /* Current numerator of the partial sum. */
    seq_t sum_den = 1;        /* Current denominator of the partial sum. */

    for (idx_t k = 0; k <= n; ++k)
    {
        pow <<= 1;                                 /* pow = 2^(k+1). */
        seq_t num_term = pow - 1;                  /* 2^(k+1) - 1. */
        seq_t den_term = (seq_t)(k + 1);           /* (k+1). */

        /* Add the new term:  a/b + num_term/den_term  */
        seq_t new_num = sum_num * den_term + sum_den * num_term;
        seq_t new_den = sum_den * den_term;

        seq_t g = gcd_seq_local(new_num, new_den); /* Reduce fraction. */
        sum_num = new_num / g;
        sum_den = new_den / g;
    }
    return sum_num;   /* Numerator of the reduced sum S(n). */
}
