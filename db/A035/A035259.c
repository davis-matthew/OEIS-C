/*
 * A035259  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          20.
 * https://oeis.org/A035259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer whose prime factorization *
 *            contains each prime p ≡ 3,7,13,17 (mod 20) to an even exponent
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   chi_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/chi_minus_one.h"

static uint32_t *spf = NULL;          /* smallest prime factor array */
static uint32_t spf_size = 0;          /* current size of spf array */

static seq_t *seq_vals = NULL;         /* stored terms of the sequence */
static size_t seq_len = 0;             /* number of stored terms */
static size_t seq_cap = 0;             /* capacity of seq_vals */
static uint32_t current_checked = 0;   /* next integer to test */

/* Build smallest prime factor table up to limit */
static void build_spf(uint32_t limit) {
    uint32_t *new_spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();
    for (uint32_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* Check whether n has even exponent on all primes with chi = -1 */
static int eligible(uint32_t n) {
    if (n == 1) return 1;
    uint32_t tmp = n;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        if (p == 0) p = tmp;  /* safety for prime > table size */
        uint32_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if ((exp & 1) && chi_minus_one(p)) return 0;
    }
    return 1;
}

/* Ensure the sequence table has at least 'needed' terms */
static void ensure_values(size_t needed) {
    if (seq_len >= needed) return;
    if (seq_len == 0) {
        seq_cap = 10000;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_vals[seq_len++] = (seq_t)1;   /* sequence starts with 1 */
        current_checked = 2;
    }
    while (seq_len < needed) {
        if (current_checked > spf_size) {
            /* enlarge the spf table if we run out of precomputed range */
            uint32_t new_lim = spf_size * 2;
            if (new_lim < spf_size) new_lim = spf_size + 1; /* overflow guard */
            build_spf(new_lim);
        }
        if (eligible(current_checked)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)current_checked;
        }
        ++current_checked;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined for n<1, but safe fallback */
    if (!spf) build_spf(5000000);         /* initial sieve */
    ensure_values((size_t)n);
    return seq_vals[n - 1];
}
