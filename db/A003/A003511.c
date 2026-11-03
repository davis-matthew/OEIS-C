/*
 * A003511  A Beatty sequence: floor( n * (1 + sqrt(3))/2 ).
 * https://oeis.org/A003511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + floor(sqrt(3*n^2)))/2), equivalent to
 *            floor(n*(1+sqrt(3))/2)
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9810 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 a003511_isqrt3(uint64_t n) {
    unsigned __int128 x = (unsigned __int128)n * n * 3u;
    unsigned __int128 g = (unsigned __int128)n * 2u; /* g >= sqrt(3)*n */
    if (g == 0) return 0;
    for (;;) {
        unsigned __int128 y = (g + x / g) / 2u;
        if (y >= g) break;
        g = y;
    }
    return g;
}

static seq_t a003511_isqrt3_seq(idx_t n) {
    seq_t x = (seq_t)n * (seq_t)n * 3;
    seq_t g = (seq_t)n * 2;
    for (;;) {
        seq_t y = (g + x / g) / 2;
        if (y >= g) break;
        g = y;
    }
    return g;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < ((idx_t)1 << 61)) {
        unsigned __int128 m = a003511_isqrt3((uint64_t)n);
        uint64_t r = (uint64_t)(((unsigned __int128)n + m) / 2u);
        return (seq_t)r;
    }
    return ((seq_t)n + a003511_isqrt3_seq(n)) / 2;
}
