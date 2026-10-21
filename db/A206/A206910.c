/*
 * A206910  Position of 2n+sin(n) when the sets {2k+cos(k)} and {2k+sin(k)}
 *          are jointly ranked.
 * https://oeis.org/A206910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 1 + [cos(n) <= sin(n)] for n >= 1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The comparison cos(n) <= sin(n) is equivalent to checking whether the fractional
 * part of n/π is at least 1/4.  φ = π is irrational; we compute it by
 * integer arithmetic with a 64‑bit scaling factor.  */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t SCALE     = 1000000000000000000ULL;          /* 10^18 */
static const uint64_t INV_PI    = 318309886183790671ULL;          /* floor(SCALE / π) */
static const uint64_t QUARTER   = SCALE / 4;                      /* SCALE/4 = 2.5·10^17 */

seq_t a(idx_t n)
{
    /* Compute k = floor(n/π) and the fractional part scaled by SCALE. */
    __uint128_t prod = (__uint128_t)n * INV_PI;
    uint64_t k   = (uint64_t)(prod / SCALE);
    uint64_t rem = (uint64_t)(prod % SCALE);

    /* rem represents f·SCALE with f = fractional part of n/π. */
    bool f_ge_quarter = (rem >= QUARTER);

    /* Indicator of cos(n) <= sin(n):  true when (k even & f ≥ 1/4) or
     * (k odd  & f < 1/4).  */
    bool indicator = ((k & 1) == 0) ? f_ge_quarter : !f_ge_quarter;

    /* Compute a(n) = 2*n - 1 + indicator */
    seq_t res = (seq_t)n * 2;
    res -= 1;
    if (indicator) res += 1;
    return res;
}
