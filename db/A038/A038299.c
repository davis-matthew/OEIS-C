/*
 * A038299  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*9^j.
 * https://oeis.org/A038299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n)=binomial(e,k)*9^e where * e = floor((sqrt(1+8n)-1)/2) and k
 *            = n - e*(e+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the unique e (row index) such that e*(e+1)/2 <= n < (e+1)*(e+2)/2. */
    idx_t low = 0;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t e = low;

    /* Compute the offset k within the row. */
    idx_t tri = (unsigned __int128)e * (e + 1) / 2;
    idx_t k = n - tri;

    /* a(n) = binomial(e,k) * 9^e. */
    seq_t coeff = binomial(e, k);
    seq_t pow9  = pow_int(9, e);
    return seq_mul(coeff, pow9);
}
