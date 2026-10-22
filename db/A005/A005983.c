/*
 * A005983  Number of 4 up, 4 down, 4 up, ... permutations of length 4n+1.
 * https://oeis.org/A005983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of length 4n+1 whose shape is 4
 *            ascents, then 4 descents, then 4 ascents, and so on. Computed
 *            with the Entringer-style DP: f_i(j) = number of valid prefixes
 *            of length i+1 whose last element has rank j. An ascent step
 *            gives f_{i+1}(j) = Sum_{k<j} f_i(k); a descent ste...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *row = NULL;
static idx_t rowcap = 0;
static idx_t steps = 0;
static seq_t *vals = NULL;
static idx_t nv = 0, vcap = 0;

seq_t a(idx_t n)
{
    if (n < nv) return vals[n];
    idx_t need = 4 * n + 1;
    if (rowcap < need) {
        row = grow_zero(row, rowcap, need);
        rowcap = need;
    }
    if (vcap < n + 1) {
        vals = grow_zero(vals, vcap, n + 1);
        vcap = n + 1;
    }
    if (nv == 0) {
        row[0] = 1;
        steps = 0;
        vals[0] = 1;
        nv = 1;
    }
    while (nv <= n) {
        int t;
        for (t = 0; t < 4; t++) {
            idx_t i = steps;          /* row currently has i+1 entries */
            int up = ((steps / 4) % 2) == 0;
            if (up) {
                seq_t s = 0;
                idx_t j;
                for (j = 0; j <= i; j++) {
                    seq_t tmp = row[j];
                    row[j] = s;
                    s += tmp;
                }
                row[i + 1] = s;
            } else {
                seq_t s = 0;
                idx_t j;
                for (j = i + 1; j-- > 0;) {
                    s += row[j];
                    row[j] = s;
                }
                row[i + 1] = 0;
            }
            steps++;
        }
        {
            seq_t tot = 0;
            idx_t j;
            for (j = 0; j <= steps; j++) tot += row[j];
            vals[nv] = tot;
            nv++;
        }
    }
    return vals[n];
}
