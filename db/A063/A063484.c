/*
 * A063484  Numbers k such that Omega(k+1) = 2*Omega(k), where Omega(k) is the
 *          number of prime divisors of k (with repetition).
 * https://oeis.org/A063484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that Omega(k+1)=2*Omega(k) */
/* Computes a(n) as the nth element of A063484 */

#include <stdlib.h>
#include <stdint.h>

/* Static state for memoisation */
static seq_t   *vals      = NULL;   /* array of found terms */
static size_t   vals_cap  = 0;      /* capacity of vals */
static idx_t    vals_len  = 0;      /* number of stored terms */

static uint32_t *spf       = NULL;  /* smallest prime factor for each integer */
static idx_t    spf_cap    = 0;     /* current size of spf (indexable 0..spf_cap) */
static unsigned char *omega = NULL; /* Omega values (count of prime factors) */
static idx_t    omega_cap  = 0;     /* current size of omega array */
static idx_t    last_checked = 0;   /* last k that was examined */

/* Grow the dynamic array 'vals' if necessary */
static void ensure_vals_capacity(void) {
    if (vals_len + 1 > vals_cap) {
        size_t new_cap = vals_cap ? vals_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        vals = tmp;
        vals_cap = new_cap;
    }
}

/* Allocate and fill spf and omega arrays up to 'limit' (inclusive) */
static void build_sieve(idx_t limit) {
    /* free old arrays if present */
    free(spf);
    free(omega);
    /* allocate new arrays */
    spf    = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    omega  = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (spf == NULL || omega == NULL) exit(EXIT_FAILURE);

    /* Sieve for spf */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;            /* i is prime */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Compute Omega array */
    omega[0] = omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t rem = i / spf[i];
        omega[i] = omega[rem] + 1;
    }

    spf_cap   = limit + 1;
    omega_cap = limit + 1;
}

/* Scan for new terms up to 'limit-1' (so k+1 <= limit) */
static void scan_up_to(idx_t limit) {
    if (limit <= last_checked) return;
    for (idx_t k = last_checked + 1; k < limit; ++k) {
        unsigned char od = omega[k];
        unsigned char od1 = omega[k + 1];
        if (od1 == 2 * od) {
            ensure_vals_capacity();
            vals[vals_len++] = (seq_t)k;
        }
    }
    last_checked = limit - 1;
}

/* Ensures that at least 'n' terms have been generated */
static void ensure_n_terms(idx_t n) {
    while (vals_len < n) {
        /* Double search limit; start with a reasonable base */
        idx_t new_limit = spf_cap > 0 ? spf_cap * 2 : 1000;
        if (new_limit <= last_checked + 1) new_limit = last_checked + 2;
        build_sieve(new_limit);
        scan_up_to(new_limit);
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0, but safe */
    ensure_n_terms(n);
    return vals[n - 1];
}
