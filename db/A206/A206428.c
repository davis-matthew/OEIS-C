/*
 * A206428  Rectangular array, a(m,n) = 2^(m-1)*(3^n-1), read by
 *          antidiagonals.
 * https://oeis.org/A206428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^(i-1)*(3^j-1) read along antidiagonals of the
 *            rectangular array defined by a(m,n) = 2^(m-1)*(3^n-1). The index
 *            n is mapped to a pair (i,j) by the rule that for each k ≥ 0, the
 *            terms with i+j = k are listed in order i = 0,1,…,k. The function
 *            correctly handles any n ≥ 0 using only integer...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _diagonal_index, _prefix_before_diagonal, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/_diagonal_index.h"
#include "../utils/_prefix_before_diagonal.h"

seq_t a(idx_t n)
{
    /* find diagonal index k such that sum_{i=0}^{k-1} (i+1) <= n < sum_{i=0}^{k} (i+1) */
    idx_t k = _diagonal_index(n);
    idx_t before = _prefix_before_diagonal(k);
    idx_t d = n - before;          /* position inside diagonal k   */
    idx_t i = d;                   /* row index                    */
    idx_t j = k - d;               /* column index                 */

    /* compute 3^j - 1 */
    seq_t val = pow3(j);
    val -= 1;

    if (i == 0) {
        /* 2^(i-1) with i = 0 means divide by 2 */
        val >>= 1;
    } else {
        /* multiply by 2^(i-1) */
        val = val * pow2(i - 1);
    }

    return val;
}
