/*
 * A169667  The classical Lie superfactorial of types E6, E7, E8.
 * https://oeis.org/A169667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, bref
 * Formula:   a(1)=1!*4!*5!*7!*8!*11!, a(2)=1!*5!*7!*9!*11!*13!*17!,
 *            a(3)=1!*7!*11!*13!*17!*19!*23!*29!
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Validated: full, pass, 3 terms through n = 3 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static seq_t compute_sf(const idx_t *exps, size_t len)
{
    seq_t prod = 1;
    for (size_t i = 0; i < len; ++i)
        prod *= factorial(exps[i]);
    return prod;
}

seq_t a(idx_t n)
{
    switch (n) {
    case 1: {
        static const idx_t exps_E6[] = {1, 4, 5, 7, 8, 11};
        return compute_sf(exps_E6, sizeof exps_E6 / sizeof exps_E6[0]);
    }
    case 2: {
        static const idx_t exps_E7[] = {1, 5, 7, 9, 11, 13, 17};
        return compute_sf(exps_E7, sizeof exps_E7 / sizeof exps_E7[0]);
    }
    case 3: {
        static const idx_t exps_E8[] = {1, 7, 11, 13, 17, 19, 23, 29};
        return compute_sf(exps_E8, sizeof exps_E8 / sizeof exps_E8[0]);
    }
    default:
        return 0;  /* sequence defined only for n=1,2,3 */
    }
}
