/*
 * A231722  Partial sums of A001088 starting from its second term; a(1)=0,
 *          a(n) = A001088(2)+...+A001088(n).
 * https://oeis.org/A231722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=2..n} ∏_{j=1..k} φ(j) (partial sums of A001088
 *            starting with its second term)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t phi_i = calc_phi((seq_t)i);   /* φ(i) */
        prod = prod * phi_i;                /* prod = A001088(i) */
        if (i >= (idx_t)2) {
            sum = sum + prod;                /* accumulate from the second term */
        }
    }

    return sum;
}
