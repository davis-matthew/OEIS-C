/*
 * A007469  Shifts left 2 places under Stirling2 transform.
 * https://oeis.org/A007469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+2) = Sum_{k=1..n} Stirling2(n,k)*a(k), a(1) = a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *vals = NULL;   /* vals[i] = a(i) for 1 <= i <= have */
static seq_t *srow = NULL;   /* srow[k] = Stirling2(have-2, k) once have >= 3 */
static idx_t have = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap * 2;
    if (nc < need) nc = need;
    seq_t *nv = alloc_zero(nc);
    seq_t *nr = alloc_zero(nc);
    for (idx_t i = 0; i < cap; i++) {
        nv[i] = vals[i];
        nr[i] = srow[i];
    }
    if (vals) release(vals, cap);
    if (srow) release(srow, cap);
    vals = nv;
    srow = nr;
    cap = nc;
}

seq_t a(idx_t n) {
    if (n <= have) return vals[n];
    ensure_cap(n + 2);
    for (idx_t i = have + 1; i <= n; i++) {
        if (i <= 2) {
            vals[i] = 1;
        } else if (i == 3) {
            srow[0] = 0;
            srow[1] = 1;
            vals[i] = 1;
        } else {
            idx_t m = i - 2;
            /* advance srow from Stirling2(m-1, .) to Stirling2(m, .) */
            for (idx_t k = m; k >= 1; k--) {
                srow[k] = (seq_t)k * srow[k] + srow[k - 1];
            }
            srow[0] = 0;
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; k++) {
                sum += srow[k] * vals[k];
            }
            vals[i] = sum;
        }
        have = i;
    }
    return vals[n];
}
