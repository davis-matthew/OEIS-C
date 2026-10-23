/*
 * A140412  Determinants of the n X n matrices whose (i,j)-elements are
 *          lcm(i^2, j^2).
 * https://oeis.org/A140412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{k=1..n} [(-1)^{ω(k)} * ∏_{p|k} (p^2-1) *
 *            p^{2·e_p}]
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* smallest prime factor array builder */
static void compute_spf(idx_t limit, idx_t *spf) {
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

static idx_t *g_spf = NULL;
static idx_t g_spf_size = 0;

/* main sequence function */
seq_t a(idx_t n) {
    /* build or resize the spf table as needed */
    if (n + 1 > g_spf_size) {
        free(g_spf);
        g_spf = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
        if (!g_spf) return 0;           /* out of memory: return 0 as fallback */
        g_spf_size = n + 1;
        compute_spf(n, g_spf);
    }

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t tk = 1;
        idx_t temp = k;
        int omega = 0;

        while (temp > 1) {
            idx_t p = g_spf[temp];
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            ++omega;

            seq_t term = (seq_t)p * (seq_t)p;   /* p^2 */
            term -= 1;                          /* p^2 - 1 */
            tk *= term;

            /* multiply by p^(2*cnt) */
            seq_t ppow = 1;
            for (int i = 0; i < cnt; ++i) {
                ppow *= (seq_t)p * (seq_t)p;  /* p^2 each time */
            }
            tk *= ppow;
        }

        if (omega & 1) tk = -tk;    /* sign = (-1)^{ω(k)} */
        prod *= tk;
    }

    return prod;
}
