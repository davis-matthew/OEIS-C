/*
 * A000828  E.g.f. cos(x)/(cos(x) - sin(x)).
 * https://oeis.org/A000828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*A000111(n) for n >= 1, a(0) = 1, where A000111(n)
 *            = b(n,n) from the Entringer/boustrophedon triangle b(0,0) = 1,
 *            b(k,0) = 0 for k > 0, b(k,j) = b(k,j-1) + b(k-1,k-j)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *row = 0;   /* current triangle row k, entries 0..k */
static seq_t *nxt = 0;   /* scratch row */
static seq_t *E = 0;     /* E[i] = A000111(i) for i <= k */
static idx_t cap = 0;
static idx_t k = 0;

static void ensure_cap(idx_t need) {
    idx_t newcap, i;
    seq_t *r2, *n2, *e2;
    if (need <= cap) return;
    newcap = cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    r2 = alloc_zero(newcap);
    n2 = alloc_zero(newcap);
    e2 = alloc_zero(newcap);
    if (cap == 0) {
        r2[0] = 1;
        e2[0] = 1;
        k = 0;
    } else {
        for (i = 0; i <= k; i++) {
            r2[i] = row[i];
            e2[i] = E[i];
        }
        release(row, cap);
        release(nxt, cap);
        release(E, cap);
    }
    row = r2;
    nxt = n2;
    E = e2;
    cap = newcap;
}

seq_t a(idx_t n) {
    idx_t i, j;
    seq_t r;
    seq_t *t;
    if (n == 0) return 1;
    ensure_cap(n + 1);
    while (k < n) {
        nxt[0] = 0;
        for (j = 1; j <= k + 1; j++)
            nxt[j] = nxt[j - 1] + row[k + 1 - j];
        t = row; row = nxt; nxt = t;
        k++;
        E[k] = row[k];
    }
    r = E[n];
    for (i = 1; i < n; i++)
        r = r + r;
    return r;
}
