/*
 * A082850  Let S(0) = {}, S(n) = {S(n-1), S(n-1), n}; sequence gives
 *          S(infinity).
 * https://oeis.org/A082850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = a(n-1) with the recursion a(2^m-1) = m, a(n) = a(n - (2^m
 *            - 1)) for n > 2^m-1, where 2^m <= n+1 < 2^(m+1). Implements
 *            A082850 (OEIS)
 * Range:     u8 n <= 9971, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ⌊log2(x)⌋ for x>0, without using floating point. */
static unsigned floor_log2_uint64(uint64_t x)
{
    unsigned r = 0;
    if (x >= ((uint64_t)1 << 32)) { x >>= 32; r += 32; }
    if (x >= ((uint64_t)1 << 16)) { x >>= 16; r += 16; }
    if (x >= ((uint64_t)1 << 8))  { x >>= 8;  r += 8; }
    if (x >= ((uint64_t)1 << 4))  { x >>= 4;  r += 4; }
    if (x >= ((uint64_t)1 << 2))  { x >>= 2;  r += 2; }
    if (x >= ((uint64_t)1 << 1))  {         r += 1; }
    return r;
}

/* sequence function for OEIS A082850 */
seq_t a(idx_t n)
{
    /* n is 1‑based; the function is used only for n >= 1. */
    idx_t cur = n;
    while (1) {
        /* u = cur + 1 > 1 */
        idx_t u = cur + 1ULL;
        /* test if u is a power of two */
        if ((u & (u - 1ULL)) == 0ULL) {
            return (seq_t)floor_log2_uint64(u);  /* returns m where u = 2^m */
        }
        unsigned m = floor_log2_uint64(u);
        idx_t pow = ((idx_t)1ULL) << m;          /* 2^m <= u < 2^(m+1) */
        cur = cur - (pow - 1ULL);                /* cur = k where n = (2^m-1)+k */
    }
}
