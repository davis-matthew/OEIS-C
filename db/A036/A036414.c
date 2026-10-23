/*
 * A036414  Number of empty intervals when fractional_part(i*phi) for i = 1,
 *          ..., n is plotted along [ 0, 1 ] subdivided into n equal regions.
 * https://oeis.org/A036414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n - | { floor(n * fractional_part(i * φ)) : i = 1 .. n }
 *            |, where φ = (1 + sqrt(5)) / 2. This counts the number of empty
 *            subintervals of length 1/n among the points {
 *            fractional_part(iφ) }
 * Range:     u8 n <= 1803, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned __int128 value (floor sqrt). */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        uint64_t nxt = r | bit;
        if ((unsigned __int128)nxt * nxt <= x)
            r = nxt;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but safe */
    uint8_t *seen = (uint8_t*)calloc((size_t)n, 1);
    if (!seen) return 0;                 /* allocation failure */

    for (idx_t ii = 1; ii <= n; ++ii) {
        uint64_t t = (uint64_t)n * (uint64_t)ii;           /* t = n*i   */
        uint64_t prod = 5ULL * t * t;                      /* 5*t*t     */
        uint64_t sq = isqrt_u128((unsigned __int128)prod); /* floor(t*sqrt5) */
        uint64_t floor_phi = (t + sq) / 2;                 /* floor(t*φ) */
        idx_t slot = floor_phi % n;                       /* index of interval */
        seen[slot] = 1;
    }

    idx_t empty = 0;
    for (idx_t i = 0; i < n; ++i)
        if (!seen[i]) ++empty;

    free(seen);
    return (seq_t)empty;
}
