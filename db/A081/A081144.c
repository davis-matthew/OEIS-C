/*
 * A081144  Starting at 1, four-fold convolution of A000400 (powers of 6).
 * https://oeis.org/A081144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n-3) * binomial(n,3) for n ≥ 3, a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* 6^(n-3) via binary exponentiation */
    idx_t exp = n - 3;
    seq_t pow = 1;
    seq_t base = 6;
    while (exp)
    {
        if (exp & 1)
            pow *= base;
        base *= base;
        exp >>= 1;
    }

    /* binomial(n,3) = n*(n-1)*(n-2)/6   (using seq_t arithmetic) */
    seq_t temp = (seq_t)n * (seq_t)(n - 1);
    temp *= (seq_t)(n - 2);
    seq_t comb = temp / 6;   /* exact division */

    return pow * comb;
}
