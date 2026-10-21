/*
 * A050141  a(n) = 2*floor((n+1)*phi) - 2*floor(n*phi) - 1 where phi = (1 +
 *          sqrt(5))/2 is the golden ratio.
 * https://oeis.org/A050141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*floor((n+1)*phi) - 2*floor(n*phi) - 1 * Computation
 *            uses only integer arithmetic. A suitably large * multiple of φ
 *            (the golden ratio) is formed with * integer square‑root of 5*D²,
 *            guaranteeing that every floor * is computed exactly for all n
 *            well below 2·D
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_isqrt128.h"

static const uint64_t _D = 1ULL << 30;            /* D = 2^30  */

/* One‑time precomputation of φʰ = floor(φ·D), where φ = (1+√5)/2.
   All values are computed with 128‑bit arithmetic. */
static uint64_t _phi_scaled(void)
{
    static uint64_t phi = 0;
    if (!phi) {
        __int128 sx = (__int128)5 * _D * _D;          /* 5·D²  */
        uint64_t sqrt5 = _isqrt128(sx);              /* floor(D·√5) */
        phi = (_D + sqrt5) >> 1;                     /* floor((D+√5·D)/2) */
    }
    return phi;
}

/* Computes floor(n·φ) using integer arithmetic only. */
static uint64_t _floor_n_phi(uint64_t n)
{
    const uint64_t phi = _phi_scaled();
    return (uint64_t)(((__int128)n * phi) / _D);
}

seq_t a(idx_t n)
{
    /* 1‑based indexing, n ≥ 1  */
    uint64_t n0 = (uint64_t)n;
    uint64_t t0 = _floor_n_phi(n0);
    uint64_t t1 = _floor_n_phi(n0 + 1);
    uint64_t ans = 2 * (t1 - t0) - 1;
    return (seq_t)ans;
}
