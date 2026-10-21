/*
 * A055237  Sums of two powers of 5.
 * https://oeis.org/A055237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 5^k + 5^l, * where k = floor((sqrt(8*n+1)-1)/2) and * l =
 *            n - k*(k+1)/2
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 104, u64 n <= 405,
 *            u128 n <= 1594, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static idx_t triangular_inverse(idx_t n)
{
    /* Find the largest k such that k*(k+1)/2 <= n. */
    idx_t k = 0;
    while (1)
    {
        /* Use a 128‑bit intermediate to avoid overflow for large n. */
        unsigned __int128 t = (unsigned __int128)k * (k + 1) / 2;
        if (t > n)
            break;
        ++k;
    }
    return k - 1;
}

seq_t a(idx_t n)
{
    idx_t k = triangular_inverse(n);
    idx_t l = n - (k * (k + 1)) / 2;

    seq_t term_k = pow_uint(5, k);
    seq_t term_l = pow_uint(5, l);

    return term_k + term_l;
}
