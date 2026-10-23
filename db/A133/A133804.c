/*
 * A133804  Triangle read by rows: A007318 * A133080 * A133566 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A133804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) + binomial(r,k+1) + (k odd ?
 *            binomial(r,k+2) : 0) where r = ⌊(√(8n+1) – 1)/2⌋ and k = n –
 *            r(r+1)/2. This flattens the triangular array defined by T(n,k) =
 *            binomial(n,k) + binomial(n,k+1) + (binomial(n,k+2) –
 *            (–1)^k·binomial(n,k+2))/2
 * Range:     u8 n <= 38, u16 n <= 144, u32 n <= 543, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row index r by incrementing until the triangular number
       exceeds n.  Use 128‑bit arithmetic for safety. */
    idx_t r = 0;
    while (1) {
        __uint128_t next = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (next > n)
            break;
        ++r;
    }

    /* k = n – r(r+1)/2 */
    __uint128_t start128 = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start128;

    /* Compute the triangle entry using the simplified formula. */
    seq_t term = binomial(r, k);

    if (k + 1 <= r)
        term += binomial(r, k + 1);

    if ((k & 1ULL) && (k + 2 <= r))
        term += binomial(r, k + 2);

    return term;
}
