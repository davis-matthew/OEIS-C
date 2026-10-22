/*
 * A007136  Number of symmetry sites in all planted 3-trees with n nodes.
 * https://oeis.org/A007136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with B(m) = number of rooted trees on m nodes in which every
 *            node has at most 2 children (B(0)=0, B(1)=1, B(m) = B(m-1) +
 *            Sum_{i<j, i+j=m-1} B(i)B(j) + [m-1 even] C(B((m-1)/2)+1, 2)) and
 *            S(m) = total number of symmetric nodes (two isomorphic child
 *            subtrees) over those trees (S(0)=S(1)=0, S(m) = S...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   choose2rep, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/choose2rep.h"

static seq_t *bb = NULL;
static seq_t *ss = NULL;
static idx_t have = 0; /* entries 0..have-1 are valid */

static void extend(idx_t len) {
    idx_t m, i;
    if (len <= have) return;
    bb = grow_zero(bb, have, len);
    ss = grow_zero(ss, have, len);
    for (m = have; m < len; m++) {
        idx_t k;
        seq_t p = 0, q = 0;
        if (m == 0) { bb[0] = 0; ss[0] = 0; continue; }
        if (m == 1) { bb[1] = 1; ss[1] = 0; continue; }
        k = m - 1;
        for (i = 1; 2 * i < k; i++)
            p += bb[i] * bb[k - i];
        for (i = 1; i < k; i++)
            q += ss[i] * bb[k - i];
        if (k % 2 == 0) {
            p += choose2rep(bb[k / 2]);
            q += ss[k / 2] + bb[k / 2];
        }
        bb[m] = bb[k] + p;
        ss[m] = ss[k] + q;
    }
    have = len;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    extend(n);
    return ss[n - 1];
}
