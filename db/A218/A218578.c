/*
 * A218578  The number of times n occurs in A095972.
 * https://oeis.org/A218578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k >= 1 with A095972(k) = n, i.e. k - A000224(k)
 *            = n; A000224 is multiplicative with a(2^e)=floor(2^e/6)+2,
 *            a(p^e)=floor(p^(e+1)/(2p+2))+1 for odd p; k <= 2n+2 suffices,
 *            since A000224(k) <= k/2+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9461 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A218578_sq(uint64_t p, uint64_t q)
{
    if (p == 2) return q / 6 + 2;
    return (uint64_t)(((unsigned __int128)q * p) / (2 * (unsigned __int128)p + 2)) + 1;
}

static uint64_t A218578_nonres(uint64_t k)
{
    uint64_t m = k, r = 1, p;
    if (m <= 1) return 0;
    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t q = 1;
            while (m % p == 0) { m /= p; q *= p; }
            r *= A218578_sq(p, q);
        }
    }
    if (m > 1) r *= A218578_sq(m, m);
    return k - r;
}

seq_t a(idx_t n)
{
    uint64_t lim = 2 * (uint64_t)n + 4, k, cnt = 0;
    for (k = 1; k <= lim; k++) {
        if (A218578_nonres(k) == (uint64_t)n) cnt++;
    }
    return (seq_t)cnt;
}
