/*
 * A000338  Expansion of x^3*(5-2*x)*(1-x^3)/(1-x)^4.
 * https://oeis.org/A000338
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*C(n-1,2) + 3*C(n-2,2) + 3*C(n-3,2) - 2*C(n-4,2) where
 *            C(k,2) = k*(k-1)/2
 * Range:     u8 n <= 9, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom2(seq_t k)
{
    if (k % 2 == 0)
        return (k / 2) * (k - 1);
    else
        return k * ((k - 1) / 2);
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    
    if (n >= 3)
        result += 5 * binom2((seq_t)(n - 1));
    if (n >= 4)
        result += 3 * binom2((seq_t)(n - 2));
    if (n >= 5)
        result += 3 * binom2((seq_t)(n - 3));
    if (n >= 6)
        result -= 2 * binom2((seq_t)(n - 4));
    
    return result;
}
