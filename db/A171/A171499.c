/*
 * A171499  a(n) = 6*a(n-1) - 8*a(n-2) for n > 1; a(0) = 3, a(1) = 14.
 * https://oeis.org/A171499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2^(n+2)-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2n  = pow2_exp(n);
    seq_t p2n2 = pow2_exp(n + 2);
    p2n2 -= 1;
    return p2n * p2n2;
}
