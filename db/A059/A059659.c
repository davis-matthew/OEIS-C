/*
 * A059659  Positions of minus ones (-1's) in A059651.
 * https://oeis.org/A059659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index m >= 1 with A059651(m) = -1, where A059651(m)
 *            = floor(k^2*m) - floor(k*m) ... computed as b - c - 1,
 *            b=floor(cbrt(4m^3)), c=floor(cbrt(2*floor(cbrt(2m^3))^3)),
 *            k=2^(1/3); found by a scan with cached results
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A059659_icbrt(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 22;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static uint64_t *A059659_pos = 0;
static uint64_t A059659_cnt = 0, A059659_cap = 0, A059659_next = 1;

seq_t a(idx_t n)
{
    while (A059659_cnt < (uint64_t)n) {
        uint64_t m = A059659_next++;
        uint64_t m3 = m * m * m;
        uint64_t mm = A059659_icbrt(2 * m3);
        uint64_t b = A059659_icbrt(4 * m3);
        uint64_t mm3 = mm * mm * mm;
        uint64_t c = A059659_icbrt(2 * mm3);
        if (b == c) {
            if (A059659_cnt == A059659_cap) {
                A059659_cap = A059659_cap ? A059659_cap * 2 : 1024;
                A059659_pos = (uint64_t *)realloc(A059659_pos, A059659_cap * sizeof(uint64_t));
            }
            A059659_pos[A059659_cnt++] = m;
        }
    }
    return (seq_t)A059659_pos[n - 1];
}
