/*
 * A187818  Triangle read by rows in which row n lists the first 2^(n-1) terms
 *          of A038712 in nonincreasing order, n >= 1.
 * https://oeis.org/A187818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the (r,p)-entry of the tri‑angular array in A187818,
 *            where r = ⌊log₂(n)⌋ + 1 and p = n – (2^(r−1) – 1). The value
 *            equals 2^k – 1 for the unique k with 1 ≤ k ≤ r that places p
 *            inside the block of size 2^(r−k−1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* r = floor(log₂(n)) + 1 */
    idx_t temp = n;
    idx_t r = 0;
    while (temp) {
        temp >>= 1;
        ++r;
    }

    /* Position within row r */
    idx_t p = n - (((idx_t)1 << (r - 1)) - 1);

    /* Find the block containing p */
    for (idx_t k = r; k >= 1; --k) {
        idx_t block = (k == r) ? (idx_t)1 : ((idx_t)1 << (r - k - 1));
        if (p <= block) {
            return pow2(k) - 1;
        }
        p -= block;
    }
    /* Unreachable */
    return 0;
}
