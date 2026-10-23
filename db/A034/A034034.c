/*
 * A034034  Numbers that are primitively or imprimitively represented by
 *          x^2+2y^2, but not both.
 * https://oeis.org/A034034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: for each integer m≥0, check representations m=x^2+2y^2.
   Count m if it can be represented, but only in primitive or only in
   imprimitive form (gcd(x,y)=1 or >1 but not both).  a(1)=0. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

static inline bool has_primitive_or_imprimitive_only(seq_t m)
{
    bool has_primitive = false;
    bool has_imprimitive = false;

    /* x ranges from 0 up to floor(sqrt(m)) */
    seq_t sqrt_m = floor_sqrt(m);
    for (idx_t x = 0; x <= (idx_t)sqrt_m; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        if (x2 > m) break;

        seq_t rem = m - x2;
        /* rem must be even to be 2*y^2 */
        if (rem & (seq_t)1) continue;

        seq_t y2 = rem >> 1;
        seq_t y = floor_sqrt(y2);
        if (y * y != y2) continue;       /* not a perfect square */

        seq_t g = gcd((seq_t)x, y);
        if (g == (seq_t)1) {
            has_primitive = true;
        } else {
            has_imprimitive = true;
        }

        if (has_primitive && has_imprimitive) {
            /* both types present => exclude this m */
            return false;
        }
    }
    /* Include m only if it has exactly one type of representation */
    return has_primitive ^ has_imprimitive;
}

seq_t a(idx_t n)
{
    /* offset: a(1) = 0 */
    if (n == 0) return (seq_t)0;   /* Safe fallback for out‑of‑range, not used */

    if (n == 1) return (seq_t)0;

    idx_t count = 1;   /* a(1)=0 already counted */
    idx_t m = 1;

    for (;;) {
        seq_t sm = (seq_t)m;
        if (has_primitive_or_imprimitive_only(sm)) {
            ++count;
            if (count == n) return sm;
        }
        ++m;
    }
}
