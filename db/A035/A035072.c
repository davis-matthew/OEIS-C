/*
 * A035072  a(n) is root of square starting with digit 5: first term of runs.
 * https://oeis.org/A035072
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(sqrt(5 * 10^n)), n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* helper to compute 10^n in 128–bit unsigned integer (used only when seq_t is
 * at most 64 bits).  It is safe for n up to about 38, which covers all n
 * whose results fit into uint64_t. */
static unsigned __int128 pow10_uint128(idx_t n)
{
    unsigned __int128 r = 1;
    for (idx_t i = 0; i < n; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Branch for narrow integer types (≤ 64 bits).  For big‑int types we
     * fall back to arithmetic on seq_t. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit arithmetic for the product 5*10^n. */
        unsigned __int128 pow10n = pow10_uint128(n);
        unsigned __int128 prod   = pow10n * (unsigned __int128)5;

        /* Approximate square root with long double, then correct by
         * integer adjustments to avoid any rounding errors. */
        long double approx = sqrtl((long double)prod);
        uint64_t s = (uint64_t)floorl(approx);

        unsigned __int128 s2 = (unsigned __int128)s * (unsigned __int128)s;
        while (s2 > prod) {
            --s;
            s2 = (unsigned __int128)s * (unsigned __int128)s;
        }
        while (s2 < prod) {
            ++s;
            s2 = (unsigned __int128)s * (unsigned __int128)s;
        }
        return (seq_t)s;
    }

    /* Big‑integer / arbitrary‑precision path. */
    seq_t ten_pow = pow10(n);
    seq_t product = (seq_t)5 * ten_pow;    /* 5*10^n */
    seq_t s = floor_sqrt(product);         /* floor(sqrt(product)) */
    seq_t ss = s * s;                      /* exact square */
    if (ss == product)                     /* perfect square */
        return s;
    return s + (seq_t)1;                   /* ceil(sqrt(product)) */
}
