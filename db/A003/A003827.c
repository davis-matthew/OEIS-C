/*
 * A003827  'Core' alternating sign n X n matrices, i.e., those that are not
 *          'blown up' from a smaller matrix by inserting row i, column j with
 *          a_ij = 1 and all other entries in that row and column equal to 0.
 * https://oeis.org/A003827
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   A005130(n) = Sum_{m=0..n} binomial(n,m)^2 * (n-m)! * a(m), with
 *            a(0) = 1, a(1) = a(2) = 0, so a(n) = A005130(n) - Sum_{m=0..n-1}
 *            binomial(n,m) * n!/m! * a(m), where A005130(n) =
 *            Product_{i=0..n-1} (3i+1)!/(n+i)! is evaluated via its prime
 *            factorization
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   asm_count, grow_zero, pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/asm_count.h"
#include "../utils/pw.h"

static seq_t *memo = NULL;
static seq_t *row = NULL;   /* Pascal row for (cnt - 1) */
static idx_t cap = 0, cnt = 0;

seq_t a(idx_t n)
{
    while (cnt <= n) {
        idx_t m = cnt, k;
        if (cnt + 1 >= cap) {
            idx_t nc = 2 * cap + 16;
            memo = grow_zero(memo, cap, nc);
            row = grow_zero(row, cap, nc);
            cap = nc;
        }
        /* advance Pascal row to row m */
        if (m == 0) {
            row[0] = 1;
        } else {
            for (k = m; k >= 1; k--) row[k] += row[k - 1];
        }
        seq_t s = 0, fall = 1;
        for (k = 1; k <= m; k++) {
            fall *= (seq_t)(m - k + 1);
            s += row[k] * fall * memo[m - k];
        }
        memo[m] = asm_count(m) - s;
        cnt++;
    }
    return memo[n];
}
