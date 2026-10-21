/*
 * A068424  Triangle of falling factorials, read by rows: T(n, k) =
 *          n*(n-1)*...*(n-k+1), n > 0, 1 <= k <= n.
 * https://oeis.org/A068424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle
 *            T(r,k)=r*(r-1)*...*(r-k+1) read by rows. For n ≥ 1 find the
 *            smallest r with r(r+1)/2 ≥ n; set k = n - r(r-1)/2 (1 ≤ k ≤ r)
 *            and return T(r,k)
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 89, u64 n <= 228,
 *            u128 n <= 625, big any n
 * Helpers:   row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/row_of_index.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; behaviour for n=0 is undefined but return 0 */
    idx_t r = row_of_index(n);
    idx_t prev_terms = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - prev_terms;         /* 1 ≤ k ≤ r */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res *= (seq_t)(r - i);
    }
    return res;
}
