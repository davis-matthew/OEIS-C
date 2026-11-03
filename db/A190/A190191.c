/*
 * A190191  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(2), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·(√2 + 1/√2)⌋ – ⌊n·√2⌋ – ⌊n/√2⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 8471 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic, no floating point. */
#include <stdint.h>

/* Helper: floor(k * √2) computed by scaled integer arithmetic. */
static uint64_t floor_sqrt2(uint64_t k) {
    /* scaling factor: √2 ≈ 1.4142135623730950488 × 10^18 */
    const uint64_t SCALE = 1414213562373095048ULL;      /* floor(√2 × 10^18) */
    const uint64_t DIV   = 1000000000000000000ULL;      /* 10^18 */

    /* use 128‑bit to avoid overflow when multiplying k by SCALE */
    __int128 prod = (__int128)k * SCALE;
    return (uint64_t)(prod / DIV);
}

seq_t a(idx_t n) {
    /* compute ⌊n·√2⌋ */
    uint64_t nu = floor_sqrt2(n);

    /* compute ⌊n/√2⌋ = ⌊⌊n·√2⌋ / 2⌋, because division by 2 preserves the floor */
    uint64_t nv = nu / 2;

    /* compute ⌊n·(√2 + 1/√2)⌋ = ⌊(3n)·√2 / 2⌋ = ⌊⌊(3n)·√2⌋ / 2⌋ */
    uint64_t nu_plus_nu_1over_sqrt2 = floor_sqrt2(3 * (uint64_t)n) / 2;

    /* a(n) = ⌊(3n)·√2/2⌋ – ⌊n·√2⌋ – ⌊n/√2⌋  (always 0 or 1) */
    uint64_t val = nu_plus_nu_1over_sqrt2 - nu - nv;

    return (seq_t)val;
}
