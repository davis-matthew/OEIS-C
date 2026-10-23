/*
 * A109882  Palindromes with either no internal digits or all internal digits
 *          are 0.
 * https://oeis.org/A109882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-1 (n≤10); for n>10 let t = n-11, k = t/9+1, d = t%9+1;
 *            then a(n) = d * 10^k + d (i.e. palindromes with no internal
 *            digits or all internal digits are 0)
 * Range:     u8 n <= 21, u16 n <= 43, u32 n <= 86, u64 n <= 173,
 *            u128 n <= 346, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_idx(idx_t k)
{
    seq_t v = 1;
    for (idx_t i = 0; i < k; ++i)
        v *= 10;
    return v;
}

seq_t a(idx_t n)
{
    /* n must be ≥1 */
    if (n <= 10)
        return (seq_t)(n - 1);

    idx_t t = n - 11;
    idx_t k = t / 9 + 1;            /* number of zeros between the first
                                       equal digits (k≥1) */
    idx_t d = t % 9 + 1;            /* leading and trailing digit (1‑9) */

    seq_t powk = pow10_idx(k);      /* 10^k as seq_t */
    seq_t val = powk * (seq_t)d;    /* d * 10^k */
    val += (seq_t)d;                /* + d */

    return val;
}
