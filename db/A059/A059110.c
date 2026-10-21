/*
 * A059110  Triangle T = A007318*A271703; T(n,m)= Sum_{i=0..n}
 *          L'(n,i)*binomial(i,m), m=0..n.
 * https://oeis.org/A059110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the row‑wise concatenation of the triangle T(n,m) =
 *            Σ_{i=m}^n L'(n,i) * binomial(i,m), where L'(n,i) = (n!/i!) *
 *            binomial(n-1,i-1) for i≥1, L'(0,0)=1 and L'(n,0)=0 for n>0. The
 *            index n corresponds to the position n = r*(r+1)/2 + c, 0 ≤ c ≤ r
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 77, u64 n <= 190,
 *            u128 n <= 528, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* locate the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) high <<= 1;     /* exponential search */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;   /* column within the row */

    seq_t result = 0;

    for (idx_t i = c; i <= r; ++i) {
        seq_t Li;
        if (r == 0 && i == 0) {
            Li = 1; /* L'(0,0) */
        } else if (i == 0) {
            Li = 0; /* L'(n,0) for n>0 */
        } else {
            /* compute r!/i! as product_{k=i+1}^{r} k */
            seq_t prod = 1;
            for (idx_t k = i + 1; k <= r; ++k)
                prod *= (seq_t)k;
            seq_t comb = binomial(r - 1, i - 1);
            Li = prod * comb;
        }
        seq_t binom_ic = binomial(i, c);
        result += Li * binom_ic;
    }

    return result;
}
