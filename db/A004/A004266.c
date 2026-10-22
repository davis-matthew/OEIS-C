/*
 * A004266  a(n) = round(1000*log_2(n)).
 * https://oeis.org/A004266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(1000*log_2(n)), computed exactly in integer fixed
 *            point: log_2(n) = e + log_2(m), m = n/2^e in [1,2); the fraction
 *            bits come from repeated squaring of m
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    int e = 63;
    while (!((n >> e) & 1)) e--;
    /* mantissa m in [2^62, 2^63), representing n/2^e with 62 fractional bits */
    uint64_t m = (e <= 62) ? ((uint64_t)n << (62 - e)) : ((uint64_t)n >> 1);
    uint64_t frac = 0;
    int i;
    for (i = 0; i < 50; i++) {
        unsigned __int128 sq = (unsigned __int128)m * m;
        m = (uint64_t)(sq >> 62);
        frac <<= 1;
        if (m >= ((uint64_t)1 << 63)) {
            m >>= 1;
            frac |= 1;
        }
    }
    uint64_t L = ((uint64_t)e << 50) | frac;           /* log_2(n) * 2^50 */
    unsigned __int128 t = (unsigned __int128)L * 1000 + ((uint64_t)1 << 49);
    uint64_t k = (uint64_t)(t >> 50);
    return (seq_t)k;
}
