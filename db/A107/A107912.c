/*
 * A107912  Numbers k that require two iterations of the sigma function to be
 *          >= 2*k.
 * https://oeis.org/A107912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that sigma(k) < 2*k and
 *            sigma(sigma(k)) >= 2*k (sigma = sum of positive divisors)
 * Range:     u8 n <= 43, u16 n <= 6613, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_trial.h"

static const idx_t MAX_LIMIT = 2000000;          /* limit for precomputed SPF/sigma */
static unsigned *spf = NULL;                    /* smallest prime factor table */
static idx_t *seq = NULL;                       /* generated sequence */
static idx_t seq_size = 0, seq_cap = 0;          /* sequence array size & capacity */
static idx_t next_k = 1;                         /* next integer to test for k */
static int initialized = 0;

/* Ensure the SPFs are computed up to at least 'limit'. */
static void init_spf(void) {
    spf = (unsigned *)calloc(MAX_LIMIT + 1, sizeof(unsigned));
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = (unsigned)i;
    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                if (spf[j] == j) spf[j] = (unsigned)i;
        }
    }
}

/* Compute sigma for n <= MAX_LIMIT using the SPF table. */
static seq_t sigma_small(idx_t n) {
    seq_t res = 1;
    while (n > 1) {
        idx_t p = spf[n];
        idx_t e = 0;
        idx_t p_pow = 1;
        while (n % p == 0) {
            n /= p;
            ++e;
            p_pow *= p;
        }
        /* term = 1 + p + p^2 + ... + p^e */
        seq_t term = 1;
        seq_t curr_pow = 1;
        for (idx_t i = 0; i < e; ++i) {
            curr_pow *= p;
            term += curr_pow;
        }
        res *= term;
    }
    return res;
}

/* Compute sigma for any n within reason. */
static seq_t sigma(idx_t n) {
    if (n <= MAX_LIMIT) return sigma_small(n);
    return sigma_trial(n);
}

/* Ensure the sequence array has room for at least 'min_needed' elements. */
static void ensure_seq_capacity(idx_t min_needed) {
    if (seq_cap >= min_needed) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < min_needed) new_cap = min_needed;
    seq = (idx_t *)realloc(seq, new_cap * sizeof(idx_t));
    seq_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!initialized) {
        init_spf();
        initialized = 1;
    }
    while (seq_size < n) {
        ++next_k;
        seq_t s1 = sigma(next_k);
        if (s1 < (seq_t)next_k * 2) {
            seq_t s2 = sigma((idx_t)s1);
            if (s2 >= (seq_t)next_k * 2) {
                ensure_seq_capacity(seq_size + 1);
                seq[seq_size++] = next_k;
            }
        }
    }
    return seq[n - 1];
}
