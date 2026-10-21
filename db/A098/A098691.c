/*
 * A098691  Array T(q,n) by antidiagonals: number of self-reciprocal
 *          polynomials of degree 2*n over GF(q) (for q >= 2 and n >= 1).
 * https://oeis.org/A098691
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{d|m, d odd} μ(d) * (q^(m/d) - (q mod 2)) / (2*m)
 *            where (q,m) is obtained from the index n by antidiagonal
 *            enumeration of all pairs (q≥2, m≥1) with q+m = constant,
 *            enumerated with q descending
 * Range:     u8 n <= 33, u16 n <= 74, u32 n <= 182, u64 n <= 427,
 *            u128 n <= 1160, big any n
 * Helpers:   mobius_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius_odd.h"

/* Compute base^exp as unsigned __int128.  base is assumed to be < 2^64. */
static unsigned __int128 pow128_uint128(uint64_t base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* index below offset */

    /* ---------- Recover (q, m) from the index ---------- */
    idx_t z = n - 2;                      /* zero‑based position in the sequence */
    idx_t S = 3;                          /* current diagonal sum (q + m) */
    while (((unsigned __int128)(S - 2) * (S - 1)) / 2 <= z) ++S;
    idx_t prev = ((unsigned __int128)(S - 3) * (S - 2)) / 2;
    idx_t k = z - prev;                   /* offset within this diagonal (0‑based) */
    idx_t m = k + 1;                      /* m runs from 1 upwards */
    idx_t q = S - m;                      /* q = S - m, ranges from 2 to S-1 */

    /* ---------- Compute T(q, m) ---------- */
    signed __int128 total = 0;             /* running sum of μ(d)*(q^(m/d)-(q mod 2)) */
    for (idx_t d = 1; d <= m; d += 2) {    /* only odd divisors of m */
        if (m % d != 0) continue;
        int mu = mobius_odd(d);
        if (mu == 0) continue;

        idx_t exp = m / d;
        unsigned __int128 pow_val = pow128_uint128((uint64_t)q, exp);
        unsigned __int128 term_raw = pow_val - (unsigned __int128)(q & 1u);

        signed __int128 term_signed = (mu > 0)
            ? (signed __int128)term_raw
            : -(signed __int128)term_raw;

        total += term_signed;
    }

    signed __int128 denom = (signed __int128)(2 * m);
    signed __int128 result = total / denom;      /* division is exact */

    return (seq_t)result;
}
