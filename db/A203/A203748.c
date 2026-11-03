/*
 * A203748  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (0,1,1,2,2,...,floor(n/2)).
 * https://oeis.org/A203748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{0≤i<j≤n-1} (v_i^2 + v_i*v_j + v_j^2), where v_i =
 *            (i==0?0:(i+1)/2) (OEIS notation)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static inline idx_t count_of_value(idx_t n, idx_t val)
{
    /* Count occurrences of value `val` (0 <= val <= n/2) in
       the list (0,1,1,2,2,...,floor(n/2)) */
    if (val == 0u) return 1u;
    if (val < n/2u) return 2u;
    /* val == n/2 */
    return (n % 2u == 0u) ? 1u : 2u;
}

seq_t a(idx_t n)
{
    seq_t res = 1;          /* initial product */

    if (n <= 1u) return res;   /* a(1) = 1 */

    idx_t m = n / 2u;     /* maximum value appearing in the list */

    for (idx_t u = 0u; u <= m; ++u)
    {
        idx_t cu = count_of_value(n, u);

        /* self‑pair contribution (u,u) */
        if (cu > 1u)
        {
            idx_t self_pairs = cu * (cu - 1u) / 2u;  /* exponent */
            unsigned int base_self =
                (unsigned int)(3u * u * u);          /* 3*u^2 fits in 32 bits */

            for (idx_t k = 0u; k < self_pairs; ++k)
                res = mul_small(res, base_self);
        }

        /* cross‑pair contributions (u,v), v>u */
        for (idx_t v = u + 1u; v <= m; ++v)
        {
            idx_t cv = count_of_value(n, v);
            idx_t exp = cu * cv;                     /* exponent (0,1,2 or 4) */
            if (exp == 0u) continue;

            unsigned int base_cross =
                (unsigned int)(u * u + u * v + v * v); /* <= 75 000 000 */

            for (idx_t k = 0u; k < exp; ++k)
                res = mul_small(res, base_cross);
        }
    }

    return res;
}
