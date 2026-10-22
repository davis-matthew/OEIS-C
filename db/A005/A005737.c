/*
 * A005737  Number of fanout-free Boolean functions of n variables using And,
 *          Or and Not gates.
 * https://oeis.org/A005737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*T(k), where T(0) = T(1) = 2 and T(k)
 *            = 2^(k+1)*A000311(k) for k >= 2 (read-once And/Or/Not functions
 *            that depend on all k variables). A000311(n) =
 *            -(n-1)*A000311(n-1) + Sum_{k=1..n-1}
 *            C(n,k)*A000311(k)*A000311(n-k), with A000311(0) = 0, A000311(1)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *S = 0;      /* A000311 values */
static idx_t S_cap = 0;   /* allocated length */
static idx_t S_have = 0;  /* number of computed entries */

seq_t a(idx_t n)
{
    idx_t m, k;
    seq_t *row;
    seq_t res, p;

    if (S_cap < n + 2) {
        idx_t nc = n + 2;
        if (S_cap == 0)
            S = alloc_zero(nc);
        else
            S = grow_zero(S, S_cap, nc);
        S_cap = nc;
    }

    row = alloc_zero(n + 1);
    row[0] = 1;

    for (m = 0; m <= n; m++) {
        if (m > 0) {
            for (k = m; k >= 1; k--)
                row[k] += row[k - 1];
        }
        if (m >= S_have) {
            if (m == 0) {
                S[0] = 0;
            } else if (m == 1) {
                S[1] = 1;
            } else {
                seq_t s = 0;
                for (k = 1; k < m; k++)
                    s += row[k] * S[k] * S[m - k];
                s -= S[m - 1] * (seq_t)(m - 1);
                S[m] = s;
            }
        }
    }
    if (S_have < n + 1)
        S_have = n + 1;

    /* row now holds C(n,k) */
    res = row[0] * 2;
    if (n >= 1)
        res += row[1] * 2;
    p = 4; /* 2^(k+1) with k = 1 */
    for (k = 2; k <= n; k++) {
        p *= 2;
        res += row[k] * p * S[k];
    }

    release(row, n + 1);
    return res;
}
