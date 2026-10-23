/*
 * A147546  Expansion of (7-41*x+42*x^2)/((1-6*x)*(1-3*x)*(1-2*x)).
 * https://oeis.org/A147546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 6*3^n - 3*2^n + 4*6^n, with a(0)=7, a(1)=36, a(2)=186
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t t3 = pow_seq_u64((seq_t)3, n);
    seq_t t2 = pow_seq_u64((seq_t)2, n);
    seq_t t6 = pow_seq_u64((seq_t)6, n);

    /* 6*3^n  + 4*6^n  - 3*2^n  */
    seq_t res = (seq_t)6 * t3 + (seq_t)4 * t6;
    res -= (seq_t)3 * t2;
    return res;
}
