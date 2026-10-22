/*
 * A004532  Theta series of {D_10}^{+} lattice.
 * https://oeis.org/A004532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Expansion of (theta_2(q)^10 + theta_3(q)^10 + theta_4(q)^10)/2
 *            in powers of q^(1/2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 271, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

typedef long long llint;

static void poly_mult(llint *r, const llint *a, const llint *b, idx_t len) {
    llint *t = (llint *)calloc(len, sizeof(llint));
    for (idx_t i = 0; i < len; i++) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; i + j < len; j++) {
            if (b[j] == 0) continue;
            t[i + j] += a[i] * b[j];
        }
    }
    memcpy(r, t, len * sizeof(llint));
    free(t);
}

static void poly_pow10(llint *p, idx_t len) {
    llint *p2 = (llint *)calloc(len, sizeof(llint));
    llint *p4 = (llint *)calloc(len, sizeof(llint));
    llint *p8 = (llint *)calloc(len, sizeof(llint));
    
    poly_mult(p2, p, p, len);
    poly_mult(p4, p2, p2, len);
    poly_mult(p8, p4, p4, len);
    poly_mult(p, p8, p2, len);
    
    free(p2);
    free(p4);
    free(p8);
}

seq_t a(idx_t n) {
    idx_t len = 2 * n + 2;
    
    llint *th2 = (llint *)calloc(len, sizeof(llint));
    llint *th3 = (llint *)calloc(len, sizeof(llint));
    llint *th4 = (llint *)calloc(len, sizeof(llint));
    
    for (idx_t k = 0; ; k++) {
        idx_t idx = (2*k + 1) * (2*k + 1);
        if (idx >= len) break;
        th2[idx] = 2;
    }
    
    th3[0] = 1;
    for (idx_t k = 1; ; k++) {
        idx_t idx = 4 * k * k;
        if (idx >= len) break;
        th3[idx] = 2;
    }
    
    th4[0] = 1;
    for (idx_t k = 1; ; k++) {
        idx_t idx = 4 * k * k;
        if (idx >= len) break;
        th4[idx] = (k & 1) ? -2LL : 2LL;
    }
    
    poly_pow10(th2, len);
    poly_pow10(th3, len);
    poly_pow10(th4, len);
    
    seq_t result = (seq_t)((th2[2*n] + th3[2*n] + th4[2*n]) / 2);
    
    free(th2);
    free(th3);
    free(th4);
    
    return result;
}
