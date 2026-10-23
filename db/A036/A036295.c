/*
 * A036295  Numerator of Sum_{i=1..n} i/2^i.
 * https://oeis.org/A036295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(2 - (n+2)/2^n) = 2^(n+1-k) - m where k =
 *            v_2(n+2) and m = (n+2)/2^k (odd)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_uint_seq(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t x = n + 2;          /* n+2 */
    idx_t k = 0;              /* exponent of 2 dividing n+2 */
    while ((x & 1) == 0) {
        x >>= 1;
        ++k;
    }
    idx_t exp = n + 1 - k;    /* exponent for 2^(n+1-k) */

    seq_t pow_exp = pow_uint_seq(2, exp);      /* 2^(n+1-k) */
    seq_t pow_k   = pow_uint_seq(2, k);        /* 2^k                 */
    seq_t m       = ((seq_t)(n + 2)) / pow_k; /* odd part of n+2     */

    return pow_exp - m;                       /* 2^(n+1-k) - m       */
}
