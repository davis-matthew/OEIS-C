/*
 * A005775  Number of compact-rooted directed animals of size n having 3
 *          source points.
 * https://oeis.org/A005775
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+2)*(n-3)*a(n) = 2*n*(n-1)*a(n-1) + 3*(n-1)*(n-2)*a(n-2), a(2)
 *            = 0, a(3) = 1 (arbitrary precision); for fixed-width seq_t, a(n)
 *            = T(n-1,2) + T(n-1,3) with T(m,k) the trinomial coefficients
 *            [x^k](1+x+1/x)^m, built by additions only (exact mod 2^w)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_is_big.h"
#include "../utils/grow_zero.h"

static seq_t *vals = NULL;  /* vals[i] valid for i < nvals */
static idx_t nvals = 0;
static seq_t *row = NULL;   /* row[k] = T(m,k), k = 0..m */
static idx_t rowlen = 0;
static idx_t m = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    if (n < 3) return 0;
    if (n < nvals) return vals[n];
    vals = grow_zero(vals, nvals, n + 1);
    if (seq_is_big()) {
        i = nvals;
        if (i < 4) {
            vals[3] = 1;
            i = 4;
        }
        for (; i <= n; i++) {
            vals[i] = ((seq_t)2 * i * (i - 1) * vals[i - 1]
                       + (seq_t)3 * (i - 1) * (i - 2) * vals[i - 2])
                      / ((seq_t)(i + 2) * (i - 3));
        }
    } else {
        if (rowlen < n + 3) {
            idx_t old = rowlen;
            row = grow_zero(row, rowlen, n + 3);
            rowlen = n + 3;
            if (old == 0) {
                row[0] = 1;
                m = 0;
            }
        }
        i = nvals < 3 ? 3 : nvals;
        for (; i <= n; i++) {
            while (m < i - 1) {
                seq_t prev = row[1]; /* T(m,-1) = T(m,1) */
                for (k = 0; k <= m + 1; k++) {
                    seq_t cur = row[k];
                    row[k] = prev + cur + row[k + 1];
                    prev = cur;
                }
                m++;
            }
            vals[i] = row[2] + row[3];
        }
    }
    nvals = n + 1;
    return vals[n];
}
