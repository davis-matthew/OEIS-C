/*
 * A182849  Joint-rank array of the numbers j*(i-1+r), where r = golden ratio
 *          = (1+sqrt(5))/2, and i>=1, j>=1, by antidiagonals.
 * https://oeis.org/A182849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binsearch_diag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A182849 – joint‑rank array T(i,j) computed with
   an integer approximation of the golden ratio.  The function
   maps the linear index n (offset 1) to the pair (i,j) on the
   antidiagonals and evaluates the defining floor sum
   T(i,j)=Σ_{k≥1}⌊j*(i-1+φ)/(k-1+φ)⌋ using r≈φ≈1.6180339887
   by the rational 1618033989/1000000000. */

#include <stdint.h>
#include "../utils/binsearch_diag.h"

static const uint64_t R_NUM = 1618033989ULL;   /* approx. φ * 1e9 */
static const uint64_t R_DEN = 1000000000ULL;   /* denominator */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 */

    uint64_t diag = binsearch_diag(n);

    /* number of entries before this diagonal */
    uint64_t prev = ((diag - 1) * (diag - 2)) / 2;
    uint64_t offset = n - prev - 1;

    uint64_t i = offset + 1;
    uint64_t j = diag - i;

    /* numerator of the integer floor sum */
    __int128 num = (__int128)j * ((i - 1) * (unsigned __int128)R_DEN + R_NUM);

    seq_t sum = 0;
    uint64_t k = 1;
    while (1) {
        __int128 den = (__int128)(k - 1) * (unsigned __int128)R_DEN + R_NUM;
        if (num < den) break;
        sum += (seq_t)(num / den);
        ++k;
    }
    return sum;
}
