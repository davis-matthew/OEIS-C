/*
 * A038215  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*9^j.
 * https://oeis.org/A038215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i,j) * 2^(i-j) * 9^j, where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2 Corresponds to
 *            row‑major enumeration of the triangle T(i,j)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2, pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow_int.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find row index i such that n is within row i */
    idx_t i = 0;
    while ((unsigned __int128)(i + 1) * (i + 2) / 2 <= n)
        i++;

    idx_t triangular = (idx_t)((unsigned __int128)i * (i + 1) / 2);
    idx_t j = n - triangular;

    seq_t val = binomial(i, j);
    val = seq_mul(val, pow2(i - j));
    val = seq_mul(val, pow_int((seq_t)9, j));
    return val;
}
