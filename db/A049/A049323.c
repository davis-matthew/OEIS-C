/*
 * A049323  Triangle of coefficients of certain polynomials (exponents in
 *          increasing order), equivalent to A033842.
 * https://oeis.org/A049323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th entry of the triangle a(r,c) =
 *            binomial(r+1,c+1)*(r+1)^{c-1} (row‑major linearisation of the
 *            triangle from A033842 / A049323)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow, binomial, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"        /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, idx_t exp); */
#include "../utils/floor_isqrt_u64.h"  /* idx_t floor_isqrt_u64(idx_t n); */

/* Compute a(n) for n >= 0
   Step 1: find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   Step 2: column c = n - r*(r+1)/2.
   Step 3: compute a(r,c) using the closed form. */
seq_t a(idx_t n)
{
    /* Find row r via integer square root */
    idx_t t = floor_isqrt_u64(8 * n + 1);
    idx_t r = (t - 1) / 2;                   /* floor((sqrt(8n+1)-1)/2) */

    /* Number of elements before row r */
    idx_t prev = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t c = n - prev;                      /* 0 <= c <= r */

    /* Handle the special case m = 0 -> value 1 */
    if (c == 0)
        return (seq_t)1;

    /* a(r,c) = binomial(r+1, c+1) * (r+1)^{c-1} */
    seq_t bin = binomial(r + 1, c + 1);
    seq_t pow = big_pow((seq_t)(r + 1), c - 1);

    return bin * pow;
}
