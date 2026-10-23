/*
 * A174038  Triangle T(n, k, q) = Sum_{j>=0} q^j * floor(binomial(n, k)/2^j)
 *          with q = 3, read by rows.
 * https://oeis.org/A174038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = T(offset_row, n - offset_row, 3) where T(n, k, q) =
 *            Σ_{j>=0} q^j * floor(binomial(n, k) / 2^j), q = 3
 * Range:     u8 n <= 30, u16 n <= 82, u32 n <= 284, u64 n <= 1008,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

   
/* ------------------------------------------------------------ */
/* Integer square root for unsigned 64‑bit values (floor).      */
static inline idx_t int_sqrt_u64(idx_t v)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* largest power of two ≤ 2^64-1/4   */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Return the row number r such that offset(r) ≤ n < offset(r+1). */
static inline idx_t row_of(idx_t n)
{
    /* offset(r) = r*(r+1)/2  =>  r ≈ floor((sqrt(8n+1)-1)/2) */
    idx_t r = (int_sqrt_u64(8ULL * n + 1) - 1) >> 1;
    /* adjust if necessary */
    if ((r + 1) * (r + 2) / 2 <= n) ++r;
    if (r * (r + 1) / 2 > n) --r;
    return r;
}

/* Compute T(n, k, 3) = Σ 3^j * floor(binom(n,k)/2^j)                    */
static inline seq_t triangle_term(idx_t n, idx_t k)
{
    seq_t bin = binomial(n, k);   /* uses util function; safe for all seq_t */
    seq_t cur  = bin;
    seq_t pow3 = 1;
    seq_t B3   = 0;

    /* Evaluate bin as a base‑3 integer with the same binary digits. */
    while (cur > 0) {
        if (cur % 2)           /* if the least‑significant bit is 1 */
            B3 += pow3;
        cur /= 2;              /* shift right by 1 (floor division by 2) */
        pow3 *= 3;             /* next power of 3 */
    }

    return 3 * B3 - 2 * bin;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t offset_r = r * (r + 1) / 2;
    idx_t k = n - offset_r;
    return triangle_term(r, k);
}
