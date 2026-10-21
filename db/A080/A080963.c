/*
 * A080963  Expansion of theta_3(q)*theta_3(q^2)*theta_4(q^8) in powers of q.
 * https://oeis.org/A080963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in θ₃(q)·θ₃(q²)·θ₄(q⁸)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    idx_t exp;
    seq_t coeff;
} pair_t;

static pair_t *phi_pairs  = NULL;
static size_t  phi_cnt    = 0;
static pair_t *psi_pairs  = NULL;
static size_t  psi_cnt    = 0;
static pair_t *gam_pairs  = NULL;
static size_t  gam_cnt    = 0;

static seq_t *a_vals = NULL;   /* values a(0)…a(max_n)   */
static size_t  a_size = 0;     /* number of stored values */

/* Recompute the lists of non‑zero coefficients for the three theta functions
   up to exponent 'max_expo'.  The lists are stored in static arrays. */
static void rebuild_lists(idx_t max_expo)
{
    /* phi: theta_3(q)  -> exponents k²  */
    size_t current = 0;
    idx_t k = 0;
    while (1) {
        idx_t e = k * k;
        if (e > max_expo) break;
        seq_t c = (k == 0) ? (seq_t)1 : (seq_t)2;
        if (current == phi_cnt) {
            phi_cnt = (phi_cnt == 0) ? 1 : phi_cnt * 2;
            phi_pairs = (pair_t*)realloc(phi_pairs, phi_cnt * sizeof(pair_t));
        }
        phi_pairs[current++] = (pair_t){ e, c };
        ++k;
    }
    phi_cnt = current;

    /* psi: theta_3(q²)  -> exponents 2·k²  */
    current = 0;
    k = 0;
    while (1) {
        idx_t e = 2 * k * k;
        if (e > max_expo) break;
        seq_t c = (k == 0) ? (seq_t)1 : (seq_t)2;
        if (current == psi_cnt) {
            psi_cnt = (psi_cnt == 0) ? 1 : psi_cnt * 2;
            psi_pairs = (pair_t*)realloc(psi_pairs, psi_cnt * sizeof(pair_t));
        }
        psi_pairs[current++] = (pair_t){ e, c };
        ++k;
    }
    psi_cnt = current;

    /* gamma: theta_4(q⁸) -> exponents 8·k²  */
    current = 0;
    k = 0;
    while (1) {
        idx_t e = 8 * k * k;
        if (e > max_expo) break;
        seq_t c;
        if (k == 0) {
            c = (seq_t)1;
        } else {
            /* 2·(-1)^k */
            c = (seq_t)( (k % 2 == 0) ?  2 : -2 );
        }
        if (current == gam_cnt) {
            gam_cnt = (gam_cnt == 0) ? 1 : gam_cnt * 2;
            gam_pairs = (pair_t*)realloc(gam_pairs, gam_cnt * sizeof(pair_t));
        }
        gam_pairs[current++] = (pair_t){ e, c };
        ++k;
    }
    gam_cnt = current;
}

/* Compute a(0)…a(max_n) by triple convolution of the sparse lists.  */
static void compute_a_up_to(idx_t max_n)
{
    /* Rebuild non‑zero coefficient lists up to max_n */
    rebuild_lists(max_n);

    /* Allocate or resize array for a values */
    if (a_size < (size_t)max_n + 1) {
        seq_t *new_vals = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t));
        if (a_vals) free(a_vals);
        a_vals = new_vals;
        a_size = (size_t)max_n + 1;
    } else {
        /* zero existing values up to max_n */
        for (size_t i = 0; i <= max_n; ++i) a_vals[i] = (seq_t)0;
    }

    /* Triple nested loops over non‑zero entries */
    for (size_t i = 0; i < phi_cnt; ++i) {
        idx_t e1 = phi_pairs[i].exp;
        seq_t c1 = phi_pairs[i].coeff;
        if (e1 > max_n) continue;
        for (size_t j = 0; j < psi_cnt; ++j) {
            idx_t e2 = psi_pairs[j].exp;
            seq_t c2 = psi_pairs[j].coeff;
            idx_t sum12 = e1 + e2;
            if (sum12 > max_n) continue;
            seq_t prod12 = c1 * c2;
            for (size_t k = 0; k < gam_cnt; ++k) {
                idx_t e3 = gam_pairs[k].exp;
                seq_t c3 = gam_pairs[k].coeff;
                idx_t total = sum12 + e3;
                if (total > max_n) continue;
                a_vals[total] += prod12 * c3;
            }
        }
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;      /* safety, though idx_t unsigned */
    if ((size_t)n >= a_size) {
        compute_a_up_to(n);
    }
    return a_vals[(size_t)n];
}
