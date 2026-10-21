/*
 * A086694  A run of 2^n 1's followed by a run of 2^n 0's, for n=0, 1, 2, ...
 * https://oeis.org/A086694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - ⌊log₂(4*(n+1)/3)⌋ + ⌊log₂(n+1)⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* for uint64_t and size_t */

/* compute ⌊log₂(x)⌋ for x>0 on a 64‑bit integer */
static inline unsigned floor_log2_u64(uint64_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* the offset is 1 – values below 1 are irrelevant */
    if (n == 0) return 0;

    uint64_t m = n + 1;                  /* n+1  (>=1) */

    /* t = ⌊4*(n+1)/3⌋; use 128‑bit arithmetic to avoid overflow */
    __uint128_t mul = (__uint128_t)m * 4U;
    uint64_t t = (uint64_t)(mul / 3U);    /* integer division is truncating */

    unsigned log_m = floor_log2_u64(m);
    unsigned log_t = floor_log2_u64(t);

    int result = 1 - (int)log_t + (int)log_m;   /* 0 or 1 only */
    return (seq_t)result;
}
