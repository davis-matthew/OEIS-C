/*
 * A052784  Expansion of e.g.f.: x^3*(exp(x)-1)^3.
 * https://oeis.org/A052784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 6, otherwise (n-2)*(n-1)*n*(3^(n-3) - 3*2^(n-3)
 *            + 3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 6ULL)
        return 0;

    idx_t e = n - 3ULL;

    seq_t pow3 = pow_seq(3, e);
    seq_t pow2 = pow_seq(2, e);

    seq_t bracket = pow3 - (pow2 * 3) + 3;
    seq_t mul = ((seq_t)(n - 2)) * ((seq_t)(n - 1)) * ((seq_t)n);
    return mul * bracket;
}
