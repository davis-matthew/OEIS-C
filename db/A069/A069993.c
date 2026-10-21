/*
 * A069993  a(n) = 2^(2*n+1)*Sum_{k=1..2*n}
 *          binomial(2*n+1,k)*Bernoulli(k)/2^k.
 * https://oeis.org/A069993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(4^n - n) - 1 (valid for n >= 1, offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^n via binary exponentiation */
    seq_t pow4 = (seq_t)1;   /* 4^0 = 1 */
    seq_t base  = (seq_t)4;  /* base = 4 */
    idx_t exp = n;

    while (exp) {
        if (exp & 1U)  pow4 *= base;
        base *= base;
        exp >>= 1U;
    }

    seq_t nn  = (seq_t)n;            /* convert index to seq_t */
    seq_t res = (seq_t)2 * pow4 - (seq_t)2 * nn - 1;

    return res;
}
