/*
 * A007512  Primes of the form floor(e*10^k), i.e., formed by concatenation of
 *          an initial segment of the decimal expansion of e.
 * https://oeis.org/A007512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(e*10^(A064118(n)-1)), where A064118 = 1, 3, 7, 85,
 *            1781, 2780, 112280, 155025, ... floor(e*10^k) is computed
 *            exactly as floor(S*sum 1/i!) with S = 10^(k+G) guard digits,
 *            then divided by 10^G
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t e_floor_big(idx_t k) {
    const idx_t G = 12;
    seq_t S = 1;
    for (idx_t i = 0; i < k + G; i++) S *= 10;
    seq_t sum = S;
    seq_t term = S;
    for (idx_t i = 1;; i++) {
        term /= i; /* floor(S/i!) by nested floors */
        if (term == 0) break;
        sum += term;
    }
    /* truncation error is at most the number of terms, far below 10^G */
    seq_t g = 1;
    for (idx_t i = 0; i < G; i++) g *= 10;
    return sum / g;
}

static uint64_t e_floor_small(idx_t k) {
    /* k <= 6: use 64-bit native arithmetic with guard digits */
    const idx_t G = 18 - k;
    uint64_t S = 1;
    for (idx_t i = 0; i < k + G; i++) S *= 10;
    uint64_t sum = S, term = S;
    for (uint64_t i = 1;; i++) {
        term /= i;
        if (term == 0) break;
        sum += term;
    }
    uint64_t g = 1;
    for (idx_t i = 0; i < G; i++) g *= 10;
    return sum / g;
}

static const uint32_t A064118_tab[] = {1, 3, 7, 85, 1781, 2780, 112280, 155025};

seq_t a(idx_t n) {
    idx_t cnt = sizeof(A064118_tab) / sizeof(A064118_tab[0]);
    if (n < 1 || n > cnt) return 0;
    idx_t k = (idx_t)A064118_tab[n - 1] - 1;
    if (k <= 6) return u128_to_seq((unsigned __int128)e_floor_small(k));
    return e_floor_big(k);
}
