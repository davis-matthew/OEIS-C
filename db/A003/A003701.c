/*
 * A003701  Expansion of e.g.f. exp(x)/cos(x).
 * https://oeis.org/A003701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*E(k), where E(k) =
 *            A000364(k/2) for even k and 0 for odd k (binomial transform of
 *            the interpolated secant numbers; e.g.f. exp(x)*sec(x)). E(k) is
 *            taken from the Seidel-Entringer triangle: T(0,0)=1, T(m,0)=0,
 *            T(m,j)=T(m,j-1)+T(m-1,m-j), zigzag(m)=T(m,m). The binom...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *ent_row = NULL;   /* Entringer row m-1 */
static seq_t *ent_tmp = NULL;   /* scratch for the next Entringer row */
static seq_t *diag = NULL;      /* antidiagonal of the binomial-transform table */
static seq_t *vals = NULL;      /* computed a(0..done-1) */
static idx_t cap = 0;
static idx_t done = 0;

static void ensure_cap(idx_t len) {
    idx_t nc;
    if (cap >= len) return;
    nc = cap * 2;
    if (nc < len) nc = len;
    if (nc < 64) nc = 64;
    if (cap == 0) {
        ent_row = alloc_zero(nc);
        ent_tmp = alloc_zero(nc);
        diag = alloc_zero(nc);
        vals = alloc_zero(nc);
    } else {
        ent_row = grow_zero(ent_row, cap, nc);
        ent_tmp = grow_zero(ent_tmp, cap, nc);
        diag = grow_zero(diag, cap, nc);
        vals = grow_zero(vals, cap, nc);
    }
    cap = nc;
}

static void step(idx_t m) {
    seq_t e, carry, t;
    idx_t k, j;
    seq_t *sw;
    ensure_cap(m + 1);
    if (m == 0) {
        ent_row[0] = 1;
        diag[0] = 1;
        vals[0] = 1;
        return;
    }
    /* next Entringer row */
    ent_tmp[0] = 0;
    for (k = 1; k <= m; k++)
        ent_tmp[k] = ent_tmp[k - 1] + ent_row[m - k];
    sw = ent_row; ent_row = ent_tmp; ent_tmp = sw;

    if (m % 2 == 0) e = ent_row[m];
    else e = 0;

    /* update antidiagonal: N[0]=E(m), N[j]=D[j-1]+N[j-1] */
    carry = e;
    for (j = 0; j < m; j++) {
        t = diag[j];
        diag[j] = carry;
        carry = t + carry;
    }
    diag[m] = carry;
    vals[m] = carry;
}

seq_t a(idx_t n) {
    while (done <= n) {
        step(done);
        done++;
    }
    return vals[n];
}
