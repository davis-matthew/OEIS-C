/*
 * A059301  Number of filter bases of an n-set.
 * https://oeis.org/A059301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k)*2^(2^k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;          /* running total of the sum */
    seq_t binom = 1;        /* C(n,0) */
    seq_t t = 2;            /* 2^(2^0)  – will be squared each step   */
    seq_t termPow = 1;      /* 2^(2^0-1) = 1, the first term of the sum */

    for (idx_t k = 0; k < n; ++k) {
        /* Add the k-th summand: C(n,k) * 2^(2^k-1) */
        sum += binom * termPow;

        if (k == n - 1)
            break;          /* finished the sum */

        /* Update the binomial coefficient to C(n,k+1) :
           C(n,k+1) = C(n,k) * (n-k) / (k+1)   (exact division) */
        binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);

        /* Update the power of two.  t holds 2^(2^k); squaring gives 2^(2^(k+1))
           After squaring we divide by 2 to obtain 2^(2^(k+1)-1). */
        t = t * t;
        termPow = t / (seq_t)2;
    }
    return sum;
}
