/*
 * A115352  Concatenation of finite strings S_0, S_1, S_2, ..., where S_0 =
 *          {0} and for k >= 1, S_k is obtained from S_{k-1} by inserting the
 *          numbers 2^(k-1) through 2^k-1 after the initial 0.
 * https://oeis.org/A115352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 0 if n = 2^m – 1; otherwise, write n = 2^(m+1)+k with 0 ≤
 *            k < 2^(m+1). If k < 2^m, then a(n) = 2^m + k; else a(n) = a(k)
 * Range:     u8 n <= 511, u16 n <= 1022, u32 n <= 1022, u64 n <= 1022,
 *            u128 n <= 1022, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(x)) for x > 0 (x is at most 2^63-1 for idx_t). */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1)  /* shift right until x becomes 0 */
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */
    while (1) {
        /* n = 2^m – 1  ⟺  (n & (n + 1)) == 0   */
        if ((n & (n + 1)) == 0)
            return 0;              /* a(n) = 0 for powers of two minus one */

        /* Highest power of two <= n: 2^(m+1) */
        idx_t pw = 1ULL << floor_log2_idx(n);   /* 2^(m+1) */
        idx_t half = pw >> 1;                   /* 2^m */
        idx_t k = n - pw;                       /* n = 2^(m+1) + k */

        if (k < half)
            return (seq_t)(half + k);           /* a(n) = 2^m + k */
        /* otherwise recurse on smaller index k */
        n = k;
    }
}
