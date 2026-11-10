/*
 * A228347  Triangle of regions and compositions of the positive integers (see
 *          Comments lines for definition).
 * https://oeis.org/A228347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row n has n terms: A129760(n) zeros, then the first A006519(n)
 *            terms of row A001511(n) of A090996 (row k = leading 1's count of
 *            2^(k-1)..2^k-1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* find row m with m(m-1)/2 < n <= m(m+1)/2 */
    uint64_t lo = 1, hi = 6074000999ULL; /* m(m+1)/2 fits in 64 bits */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((uint64_t)mid * (mid + 1) / 2 >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    uint64_t m = lo;
    uint64_t p = (uint64_t)n - m * (m - 1) / 2;   /* position 1..m */
    uint64_t z = m & (m - 1);                      /* A129760(m) */
    if (p <= z) return (seq_t)0;
    uint64_t j = p - z;                            /* 1..A006519(m) */
    unsigned k = 1;                                /* A001511(m) */
    uint64_t t = m;
    while ((t & 1) == 0) { k++; t >>= 1; }
    uint64_t x = ((uint64_t)1 << (k - 1)) + j - 1;
    int msb = 63 - __builtin_clzll(x);
    unsigned cnt = 1;
    while (msb > 0 && ((x >> (msb - 1)) & 1ULL)) { cnt++; msb--; }
    return (seq_t)cnt;
}
