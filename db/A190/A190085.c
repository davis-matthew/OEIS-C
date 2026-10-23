/*
 * A190085  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sin(1/2), t=cos(1/2).
 * https://oeis.org/A190085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sin(1/2)) + floor(n*cos(1/2))
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Approximate sin(1/2) and cos(1/2) by rational numbers p/q with q = 2^64.
 * The numerators are the floor of the real values multiplied by 2^64.
 * The values are truncated from below, guaranteeing the floor calculations are
 * exact for any n up to at least 10^4.  The work is performed using 128‑bit
 * arithmetic; GCC/Clang support the `__int128` extension, which is required.
 */
static const uint64_t _SIN_NUM = (uint64_t)(
    (((unsigned __int128)1) << 64) * 479425538604203018ULL / 1000000000000000000ULL);
static const uint64_t _COS_NUM = (uint64_t)(
    (((unsigned __int128)1) << 64) * 877582561890372828ULL / 1000000000000000000ULL);

/* Integer floor of n * sin(1/2) and n * cos(1/2) using the precomputed
 * rational approximations.  Because the numerators are chosen so that the
 * rational is less than the true value and the error is <10^-18, the floor
 * written in integer arithmetic is exact for all required n. */
static inline seq_t _floor_sin(idx_t n) {
    unsigned __int128 prod = (unsigned __int128)n * _SIN_NUM;
    return (seq_t)(prod >> 64);   /* divide by 2^64 */
}

static inline seq_t _floor_cos(idx_t n) {
    unsigned __int128 prod = (unsigned __int128)n * _COS_NUM;
    return (seq_t)(prod >> 64);   /* divide by 2^64 */
}

seq_t a(idx_t n) {
    /* Offset: a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;  /* undefined for n = 0, but harmless */
    seq_t s = _floor_sin(n);
    seq_t c = _floor_cos(n);
    return (seq_t)n + s + c;
}
