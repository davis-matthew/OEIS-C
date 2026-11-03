/*
 * A140880  Triangle read by rows, T(n,k) =
 *          Gamma(n+3)/(Gamma(k+1)*Gamma(n-k+1)) for n>=0 and 0<=k<=n.
 * https://oeis.org/A140880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Gamma(n+3)/(Gamma(k+1)*Gamma(n-k+1)) * a(n) = triangle
 *            read by rows: a(index) = T(row, col) * where row r satisfies
 *            r(r+1)/2 ≤ index < (r+1)(r+2)/2 * and col = index - r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"

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

seq_t a(idx_t n)
{
    /* Compute the row number r */
    idx_t r = (isqrt((idx_t)8 * n + 1) - 1) / 2;

    /* Position within the row */
    idx_t triangular = r * (r + 1) / 2;  /* number of elements before row r */
    idx_t k = n - triangular;           /* column index 0 .. r */

    /* Binomial part */
    seq_t bin = binomial(r + 2, k);

    /* Multiply by (r+2-k)*(r+1-k) */
    seq_t res = mul_small(bin, (unsigned)(r + 2 - k));
    res = mul_small(res, (unsigned)(r + 1 - k));

    return res;
}
