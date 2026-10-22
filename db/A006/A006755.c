/*
 * A006755  The generalized Conway-Guy sequence w^{1}.
 * https://oeis.org/A006755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n+1) = 2*a(n) - a(n - r), r = round(sqrt(2*(n-1))), for n >=
 *            7, with a(0..7) = 0, 2, 3, 4, 8, 14, 25, 47
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release, round_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/round_sqrt.h"

seq_t a(idx_t n) {
    static const int init[8] = {0, 2, 3, 4, 8, 14, 25, 47};
    if (n < 8) return (seq_t)init[n];

    seq_t *v = alloc_zero(n + 1);
    for (idx_t i = 0; i < 8; i++) v[i] = (seq_t)init[i];
    for (idx_t m = 7; m < n; m++) {
        idx_t r = round_sqrt(2 * (m - 1));
        v[m + 1] = v[m] + v[m] - v[m - r];
    }
    seq_t res = v[n];
    release(v, n + 1);
    return res;
}
