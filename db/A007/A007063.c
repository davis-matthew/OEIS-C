/*
 * A007063  Main diagonal of Kimberling's expulsion array (A035486).
 * https://oeis.org/A007063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   trace the diagonal entry back through Kimberling's expulsion
 *            array. Row k+1 is x(k,k+1), x(k,k-1), x(k,k+2), x(k,k-2), ...,
 *            x(k,1), x(k,2k), x(k,2k+1), ... so position j of row k+1 comes
 *            from row k at k+m if j=2m-1<=2(k-1), at k-m if j=2m<=2(k-1),
 *            else at j+1. Row 1 is x(1,j)=j; a(n) = x(n,n)
 * Range:     u8 n <= 92, u16 n <= 9780, u32 n <= 9780, u64 n <= 9780,
 *            u128 n <= 9780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t j = (uint64_t)n;
    uint64_t k;
    for (k = (uint64_t)n - 1; k >= 1; k--) {
        /* j is a position in row k+1; map it to a position in row k */
        if (j <= 2 * (k - 1)) {
            if (j & 1) {
                uint64_t m = (j + 1) / 2;
                j = k + m;
            } else {
                uint64_t m = j / 2;
                j = k - m;
            }
        } else {
            j = j + 1;
        }
    }
    return (seq_t)j;
}
