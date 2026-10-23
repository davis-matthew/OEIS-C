/*
 * A137575  Successive structures central number of Seaborg's periodic table
 *          of the elements (extended to 32 columns) for odd rows.
 * https://oeis.org/A137575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, uned
 * Formula:   a(1)=1; for n>1 let g be the smallest integer ≥2 such that
 *            (g-1)*g*(2g-1)/3 ≥ n, then a(n)=2*(g-1)^2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sum_groups
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_groups.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Find smallest g >= 2 with sum_groups(g) >= n */
    idx_t low = 2;
    idx_t high = 2;
    /* increase high until condition holds */
    while ((seq_t)sum_groups(high) < (seq_t)n) {
        high <<= 1;
    }
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((seq_t)sum_groups(mid) >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t g = low;  /* group index */
    seq_t k = (seq_t)(g - 1);
    return 2 * k * k;
}
