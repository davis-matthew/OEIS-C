/*
 * A089732  Triangle read by rows: T(n,k) = number of peakless Motzkin paths
 *          of length n having k (1,1) steps (can be easily translated into
 *          RNA secondary structure terminology). Except for row 0, row n has
 *          ceiling(n/2) entries.
 * https://oeis.org/A089732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row,col) where row and col are chosen so that the first
 *            row0, then row1, row2, … are concatenated. T(n,k) =
 *            binomial(n-k,k) * binomial(n-k,k+1) / (n-k) for 2k <= n-1. The
 *            a(n) sequence is the nth element of this concatenation
 * Range:     u8 n <= 32, u16 n <= 76, u32 n <= 232, u64 n <= 743,
 *            u128 n <= 2626, big any n
 * Helpers:   binomial, seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The first element is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Find the row that contains the nth element. */
    idx_t row = 0;     /* current row number */
    idx_t pos = 0;     /* number of elements before this row */

    for (;;) {
        idx_t rowlen = (row == 0) ? 1 : (row + 1) / 2;   /* ceil(row/2) */

        if (n < pos + rowlen)
            break;

        pos += rowlen;
        ++row;
    }

    /* Column index within this row (0‑based). */
    idx_t k = n - pos;

    if (row == 0) /* only possible for n==0 which is handled above */
        return (seq_t)1;

    /* Compute the value T(row,k). */
    idx_t nk = row - k;                    /* n-k = row-k */
    seq_t bin1 = binomial(nk, k);          /* C(row-k,k) */
    seq_t bin2 = binomial(nk, k + 1);      /* C(row-k,k+1) */
    seq_t denom = (seq_t)nk;               /* (row-k) */

    /* Cancel common factors with the denominator to avoid overflow. */
    seq_t g;

    g = seq_gcd(bin1, denom);
    bin1 /= g; denom /= g;

    g = seq_gcd(bin2, denom);
    bin2 /= g; denom /= g;

    /* Multiply the remaining factors and divide by the (now 1) denominator. */
    seq_t prod  = seq_mul(bin1, bin2);
    seq_t value = prod / denom;   /* exact division */

    return value;
}
