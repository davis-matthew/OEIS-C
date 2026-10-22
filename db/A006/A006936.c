/*
 * A006936  Moebius transform of numbers of preferential arrangements.
 * https://oeis.org/A006936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A000670(n/d - 1) for n >= 1, a(0) = 0,
 *            where A000670(m) = Sum_{k=0..m} T(m,k), T(m,k) = k! * S2(m,k),
 *            T(0,0) = 1, T(m,0) = 0 for m >= 1, T(m,k) = k * (T(m-1,k) +
 *            T(m-1,k-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   grow_zero, moebius_mu, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/zalloc_arr.h"
#include "../utils/grow_zero.h"
#include "../utils/moebius_mu.h"

static seq_t *fubini = NULL;   /* fubini[m] = A000670(m) */
static seq_t *trow = NULL;     /* current row T(r-1, k) */
static idx_t fub_have = 0;     /* number of A000670 entries computed */
static idx_t fub_alloc = 0;    /* allocated length of both arrays */

static void fubini_ensure(idx_t m) {
    idx_t r, k;
    if (fub_have > m) return;
    if (fub_alloc < m + 1) {
        idx_t newlen = m + 1;
        if (newlen < 2 * fub_alloc) newlen = 2 * fub_alloc;
        if (fub_alloc == 0) {
            fubini = zalloc_arr(newlen);
            trow = zalloc_arr(newlen);
        } else {
            fubini = grow_zero(fubini, fub_alloc, newlen);
            trow = grow_zero(trow, fub_alloc, newlen);
        }
        fub_alloc = newlen;
    }
    if (fub_have == 0) {
        trow[0] = (seq_t)1;
        fubini[0] = (seq_t)1;
        fub_have = 1;
    }
    for (r = fub_have; r <= m; r++) {
        seq_t s = (seq_t)0;
        trow[r] = (seq_t)0;
        for (k = r; k >= 1; k--) {
            trow[k] = (seq_t)k * (trow[k] + trow[k - 1]);
            s += trow[k];
        }
        trow[0] = (seq_t)0;
        fubini[r] = s;
    }
    fub_have = m + 1;
}

seq_t a(idx_t n) {
    seq_t result = (seq_t)0;
    idx_t d;
    if (n == 0) return (seq_t)0;
    fubini_ensure(n - 1);
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            int mu = moebius_mu(d);
            if (mu > 0) result += fubini[e - 1];
            else if (mu < 0) result -= fubini[e - 1];
            if (e != d) {
                int mu2 = moebius_mu(e);
                if (mu2 > 0) result += fubini[d - 1];
                else if (mu2 < 0) result -= fubini[d - 1];
            }
        }
    }
    return result;
}
