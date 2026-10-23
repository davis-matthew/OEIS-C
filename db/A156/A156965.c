/*
 * A156965  Denominator of Euler(n, 1/25).
 * https://oeis.org/A156965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 5^(2*n) * 2^v2(n+1) where v2(k) is the number of trailing
 *            zeros of the binary representation of k. For even n, v2(n+1)=0
 *            so a(n)=5^(2*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t count_trailing_zeros(idx_t x) {
    idx_t cnt = 0;
    while ((x & (idx_t)1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

static inline seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t exp5 = (idx_t)2 * n;                 /* 5^(2*n) */
    idx_t exp2 = (n & (idx_t)1) ? count_trailing_zeros(n + (idx_t)1) : (idx_t)0;

    seq_t res = pow_seq_t((seq_t)5, exp5);
    if (exp2)
        res = res * pow_seq_t((seq_t)2, exp2);
    return res;
}
