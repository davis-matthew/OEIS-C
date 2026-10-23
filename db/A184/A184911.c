/*
 * A184911  a(n) = n + floor(n*r/t) + floor(n*s/t) with r = 2^(1/5), s = r^2,
 *          t = r^3.
 * https://oeis.org/A184911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n / 2^(2/5)) + floor(n / 2^(1/5))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Scaling factor for fixed‑point approximations.
   Must be > max n (≈10000) to keep the floor error < 0.5. */
static const uint64_t SCALE = 1U << 20; /* 1048576 > 20000 */

static seq_t beta1;   /* ≈ 1 / 2^(1/5), scaled by SCALE */
static seq_t beta2;   /* ≈ 1 / 2^(2/5), scaled by SCALE */
static bool _inited = false;

static void _init_constants(void)
{
    if (!_inited) {
        long double b1 = powl(2.0L, -1.0L/5.0L);   /* 2^(-1/5) */
        long double b2 = powl(2.0L, -2.0L/5.0L);   /* 2^(-2/5) */
        uint64_t v1 = (uint64_t)(b1 * SCALE + 0.5L);
        uint64_t v2 = (uint64_t)(b2 * SCALE + 0.5L);
        beta1 = (seq_t)v1;
        beta2 = (seq_t)v2;
        _inited = true;
    }
}

seq_t a(idx_t n)
{
    _init_constants();

    /* floor(n / 2^(1/5))  ≈  (n * beta1) / SCALE   */
    uint64_t term1 = ((uint64_t)n * (uint64_t)beta1) / SCALE;

    /* floor(n / 2^(2/5))  ≈  (n * beta2) / SCALE   */
    uint64_t term2 = ((uint64_t)n * (uint64_t)beta2) / SCALE;

    return (seq_t)n + (seq_t)term1 + (seq_t)term2;
}
