/*
 * A168551  Expansion of g.f. (1/2)*( a*(1+x)^n + b*(1-x)^(n+2)*LerchPhi(x,
 *          -n-1, 1) + c*2^(n+1)*(1-x)^(n+1)*LerchPhi(x, -n, 1/2) ), where a =
 *          1, b = -1, and c = 1, read by rows.
 * https://oeis.org/A168551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is defined by the triangular array * * T(n,k) = 1/2 * (
 *            binomial(n,k) + * Σ_{j=0}^k (-1)^(k-j) * * ( - binomial(n+2,k-j)
 *            * (j+1)^(n+1) + 2*binomial(n+1,k-j)*(2j+1)^n ) ) * * read by
 *            rows with offset 0 (a(0)=1). * This implementation uses the
 *            provided helper functions: * binomial, pow_seq, an...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 69, u64 n <= 176,
 *            u128 n <= 445, big any n
 * Helpers:   binomial, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"
#include "../utils/mul_small.h"

/* Find the row r such that the linear index n lies in row r (0‑based). */
static idx_t _row_from_index(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Determine row and column (k) in the triangular array. */
    idx_t row = _row_from_index(n);
    idx_t start = row * (row + 1) / 2;
    idx_t k = n - start;
    if (k > row / 2) k = row - k;           /* use symmetry T(row,k)=T(row,row-k) */

    seq_t bin_nk = binomial(row, k);

    seq_t neg1 = 0, pos1 = 0;   /* sums of bin1*pow1 with negative/positive sgn */
    seq_t neg2 = 0, pos2 = 0;   /* sums of bin2*pow2*2 with negative/positive sgn */

    for (idx_t j = 0; j <= k; ++j) {
        idx_t diff = k - j;                /* (-1)^(k-j) sign */

        seq_t bin1 = binomial(row + 2, diff);
        seq_t pow1 = pow_seq((seq_t)(j + 1), (int)(row + 1));          /* (j+1)^(row+1) */
        seq_t term1 = bin1 * pow1;               /* bin1*pow1 */

        seq_t bin2 = binomial(row + 1, diff);
        seq_t pow2 = pow_seq((seq_t)(2 * j + 1), (int)row);            /* (2j+1)^row   */
        seq_t term2 = bin2 * pow2;
        term2 = mul_small(term2, 2);                     /* 2*bin2*pow2 */

        if (diff & 1) {                 /* negative sign: (-1)^(k-j) = -1 */
            neg1 += term1;
            neg2 += term2;
        } else {                        /* positive sign */
            pos1 += term1;
            pos2 += term2;
        }
    }

    /* partA = neg1 + pos2, partB = pos1 + neg2  */
    seq_t partA = neg1 + pos2;
    seq_t partB = pos1 + neg2;

    seq_t temp = bin_nk + partA;
    /* Subtract partB; the result is guaranteed non‑negative for this sequence. */
    if (temp < partB) temp = 0;   /* safety, should not happen */
    else temp -= partB;

    /* Final division by 2.  The value is always even. */
    return temp >> 1;
}
