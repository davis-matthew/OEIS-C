/*
 * A118979  O.g.f: -12*x^3/(-1+x)/(-1+2*x)/(-1+3*x) =
 *          -2-2/(-1+3*x)-6/(-1+x)+6/(-1+2*x) .
 * https://oeis.org/A118979
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 - 6*2^n + 2*3^n (OEIS: A118979)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2 = pow_uint((seq_t)2, n);
    seq_t pow3 = pow_uint((seq_t)3, n);

    seq_t res = (seq_t)6;
    res -= (seq_t)6 * pow2;
    res += (seq_t)2 * pow3;
    return res;
}
