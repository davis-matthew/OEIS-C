/*
 * A191540  Dispersion of (floor(2*n*sqrt(2))), by antidiagonals.
 * https://oeis.org/A191540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = entry of the displacement array of s(k)=⌊2k√2⌋ enumerated
 *            by antidiagonals. a(1)=1, a(2)=2, a(3)=3, a(4)=5,
 * Range:     u8 n <= 21, u16 n <= 67, u32 n <= 253, u64 n <= 947,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for unsigned __int128 */
static inline unsigned long long isqrt_uint128(unsigned __int128 v)
{
    unsigned long long lo = 0;
    unsigned long long hi = (unsigned long long)-1;   /* 2^64-1 */
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* s(k) = ⌊2 k √2⌋ = ⌊√(8 k²)⌋, valid for any seq_t that fits in unsigned __int128 */
static inline seq_t s_func(seq_t k)
{
    unsigned __int128 kk = (unsigned __int128)k;
    unsigned __int128 val = kk * kk * 8;
    unsigned long long root = isqrt_uint128(val);
    return (seq_t)root;
}

/* return the i‑th element of the complement of s (t(i)) */
static inline seq_t get_t(idx_t i)
{
    idx_t count = 0;
    idx_t n = 1;
    idx_t k = 1;
    seq_t s_k = s_func((seq_t)k);

    while (count < i) {
        if (n == s_k) {
            ++k;
            s_k = s_func((seq_t)k);
        } else {
            ++count;
            if (count == i)
                return (seq_t)n;
        }
        ++n;
    }
    return (seq_t)0;   /* unreachable */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1 */

    /* find diagonal d with T(d-1) < n ≤ T(d),  T(d)=d(d+1)/2  */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    idx_t tri_prev = (d - 1) * d / 2;
    idx_t offset   = n - tri_prev - 1;      /* 0‑based offset in the diagonal */
    idx_t i = offset + 1;                  /* row index (t‑index)   */
    idx_t j = d - offset;                  /* column index (s‑times) */

    seq_t val = get_t(i);
    for (idx_t step = 1; step < j; ++step)
        val = s_func(val);                 /* apply s j-1 times */

    return val;
}
