/*
 * A002215  Number of polyhexes with n hexagons, having reflectional symmetry
 *          (see Harary and Read for precise definition).
 * https://oeis.org/A002215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2m) = A002212(m), a(2m+1) = 2*A002212(m) for m >= 1,
 *            where v(m) = A002212(m) satisfies v(0) = 1, v(1) = 1, (m+1)*v(m)
 *            = 3*(2m-1)*v(m-1) - 5*(m-2)*v(m-2) (equivalently v(m) = 3*v(m-1)
 *            + Sum_{i=1..m-2} v(i)*v(m-1-i), used for fixed-width builds)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 62, u128 n <= 118,
 *            big any n
 * Helpers:   detect_fixed_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/detect_fixed_local.h"

static seq_t *vmemo = NULL;
static idx_t vlen = 0;   /* number of valid entries v[0..vlen-1] */
static idx_t vcap = 0;
static int mode = -1;    /* 1 = fixed width, 0 = arbitrary precision */

static seq_t v_term(idx_t m) {
    if (mode < 0) mode = detect_fixed_local();
    if (m + 1 > vcap) {
        idx_t nc = vcap ? vcap : 16;
        while (nc < m + 1) nc *= 2;
        seq_t *nv = (seq_t *)malloc(nc * sizeof(seq_t));
        idx_t i;
        for (i = 0; i < vlen; i++) nv[i] = vmemo[i];
        for (i = vlen; i < nc; i++) nv[i] = 0;
        if (vmemo) free(vmemo);
        vmemo = nv;
        vcap = nc;
    }
    if (vlen == 0) {
        vmemo[0] = 1;
        vmemo[1] = 1;
        vlen = 2;
    }
    while (vlen <= m) {
        idx_t k = vlen;
        seq_t r;
        if (mode) {
            idx_t i;
            r = (seq_t)3 * vmemo[k - 1];
            for (i = 1; i + 1 < k; i++) r += vmemo[i] * vmemo[k - 1 - i];
        } else {
            seq_t p = (seq_t)3 * (seq_t)(2 * k - 1) * vmemo[k - 1];
            seq_t q = (seq_t)5 * (seq_t)(k - 2) * vmemo[k - 2];
            r = (p - q) / (seq_t)(k + 1);
        }
        vmemo[k] = r;
        vlen++;
    }
    return vmemo[m];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    {
        idx_t m = n / 2;
        seq_t v = v_term(m);
        if (n % 2 == 0) return v;
        return (seq_t)2 * v;
    }
}
