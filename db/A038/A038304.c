/*
 * A038304  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*2^j.
 * https://oeis.org/A038304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n)=binomial(i,j)*10^(i-j)*2^j where
 *            i=floor((sqrt(8*n+1)-1)/2), j=n-i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow10.h"
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute i */
    seq_t t = 8 * (seq_t)n + 1;
    seq_t s = floor_sqrt(t);
    idx_t i = (idx_t)((s - 1) / 2);

    /* compute j */
    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;

    idx_t diff = i - j;          /* i - j = exponent of 10 */

    /* value = binomial(i,j) * 10^(i-j) * 2^j */
    seq_t val = binomial(i, j);
    val = val * pow10(diff);
    val = val * pow2(j);

    return val;
}
