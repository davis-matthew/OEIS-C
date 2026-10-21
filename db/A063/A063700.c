/*
 * A063700  Numbers k such that sigma(k + d(k)) = sigma(k) + d(k), where
 *          sigma() = A000203(), d() = A000005().
 * https://oeis.org/A063700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 867, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063700: numbers k such that sigma(k + d(k)) = sigma(k) + d(k)
   Computed by scanning integers and testing the condition. */

#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;          /* least prime factor array */
static idx_t spf_limit = 0;        /* current size of spf array */

static seq_t *terms = NULL;        /* stored sequence values */
static size_t terms_cap = 0;       /* allocation size of terms */
static idx_t terms_cnt = 0;        /* number of terms stored */

static idx_t cur_k = 1;            /* current integer being tested */

/* Ensure spf array covers all numbers up to 'limit'. */
static void ensure_spf(idx_t limit) {
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Compute sigma(n) (sum of divisors) and d(n) (number of divisors). */
static void sigma_d_of(idx_t n, seq_t *sigma_out, seq_t *d_out) {
    if (n == 0) { *sigma_out = 0; *d_out = 0; return; }
    if (n == 1) { *sigma_out = 1; *d_out = 1; return; }

    ensure_spf(n);
    idx_t m = n;
    seq_t sigma_val = 1;
    seq_t d_val = 1;

    while (m > 1) {
        idx_t p = spf[m];
        idx_t exp = 0;
        seq_t power = 1;
        seq_t sum = 1;
        while (m % p == 0) {
            m /= p;
            exp++;
            power *= (seq_t)p;
            sum += power;
        }
        sigma_val *= sum;
        d_val *= (seq_t)(exp + 1);
    }
    *sigma_out = sigma_val;
    *d_out = d_val;
}

/* Generate terms until we have at least 'needed' elements. */
static void generate_terms(idx_t needed) {
    /* Pre‑allocate a large enough sieve once. */
    const idx_t PREALLOC_LIMIT = 5000000;   /* should cover >10k terms */
    if (!spf) ensure_spf(PREALLOC_LIMIT);

    while (terms_cnt < needed) {
        seq_t sigma_k, d_k;
        sigma_d_of(cur_k, &sigma_k, &d_k);

        idx_t k_plus = cur_k + (idx_t)d_k;
        seq_t sigma_kplus, dummy;
        sigma_d_of(k_plus, &sigma_kplus, &dummy);

        if (sigma_kplus == sigma_k + d_k) {
            if (terms_cnt >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = cur_k;
        }
        cur_k++;
    }
}

/* Public entry point. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 1, but safe */
    if (terms_cnt < n) generate_terms(n);
    return terms[n - 1];
}
