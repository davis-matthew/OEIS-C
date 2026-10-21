/*
 * A081118  Triangle of first n numbers per row having exactly n 1's in binary
 *          representation.
 * https://oeis.org/A081118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^(n - t*(t+1)/2) - 1) * 2^((t*t+3*t+4)/2 - n) - 1, *
 *            where t = floor((-1 + sqrt(8*n-7))/2) – OEIS A081118
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 497, u64 n <= 2017,
 *            u128 n <= 8129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2_exp(idx_t e)
{
    seq_t r = 1;
    while (e--) r *= 2;
    return r;
}

static inline idx_t isqrt(idx_t x)
{
    /* Computes floor(sqrt(x)) using an integer binary‑search style algorithm
       that never overflows even for 64‑bit values. */
    idx_t res  = 0;
    idx_t bit = (idx_t)1 << ((sizeof(idx_t) * 8 - 2) & ~1); /* highest even bit ≤ x */

    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (offset 1). */
    if (n == 0) return 0;

    /* 8*n-7 will fit in idx_t for the test range (up to thousands of n). */
    idx_t tmp = 8 * n - 7;
    idx_t s   = isqrt(tmp);            /* floor(sqrt(8*n-7)) */
    idx_t t   = (s - 1) / 2;           /* floor((-1+sqrt(8*n-7))/2) */

    /* i = n - t*(t+1)/2   and   j = (t*t+3*t+4)/2 - n. */
    idx_t i = n - (t * (t + 1)) / 2;
    idx_t j = ((t * t + 3 * t + 4) / 2) - n;

    seq_t pow_i = pow2_exp(i);
    seq_t pow_j = pow2_exp(j);

    /* a(n) = (pow_i - 1) * pow_j - 1. */
    return (pow_i - 1) * pow_j - 1;
}
