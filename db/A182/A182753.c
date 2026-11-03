/*
 * A182753  Expansion of (1 + 14*x)/(1 - 35*x^2).
 * https://oeis.org/A182753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 14 * 35^(n-1), a(2n+1) = 35^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow35(idx_t exp)
{
    seq_t base  = 35;      /* 35 as seq_t */
    seq_t result = 1;      /* initial result */
    while (exp)
    {
        if (exp & 1)        /* multiply result when current bit is set */
            result *= base;
        exp >>= 1;          /* shift exponent */
        if (exp)            /* square base for next bit */
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 explicitly */
    if (n == 1) return 1;

    /* even index: n = 2k, k = n/2, term = 14 * 35^(k-1)  */
    if ((n & 1) == 0)   /* n is even */
    {
        idx_t k   = n / 2;
        seq_t p   = pow35(k - 1);   /* 35^(k-1) */
        return p * (seq_t)14;
    }

    /* odd index: n = 2k+1, k = (n-1)/2, term = 35^k */
    idx_t k   = (n - 1) / 2;
    return pow35(k);
}
