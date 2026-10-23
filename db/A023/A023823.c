/*
 * A023823  Sum of exponents in prime-power factorization of C(3n,n+1).
 * https://oeis.org/A023823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p} e_p(C(3n,n+1)) = S(3n) - S(n+1) - S(2n-1), where
 *            S(m) = sum_{k=1}^{m} Ω(k) and Ω(k) is the number of prime
 *            factors of k counted with multiplicity
 * Range:     u8 n <= 855, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for smallest prime factors and prefix sums of Ω(k). */
static uint32_t *spf   = NULL;          /* smallest prime factor array   */
static idx_t     spf_len = 0;           /* number of entries in spf      */
static seq_t   *prefix = NULL;          /* prefix[i] = Σ_{k≤i} Ω(k)      */
static uint8_t *omega  = NULL;          /* Ω(k) for each k               */
static idx_t     prefix_len = 0;        /* number of entries in prefix   */

/* Allocate or extend the caches up to at least limit >= 0. */
static void _ensure_cache(idx_t limit)
{
    /* Ensure spf array covers [0, limit]. */
    if (spf_len <= limit) {
        size_t newlen = (size_t)(limit + 1);
        uint32_t *new_spf;
        if (spf == NULL) {
            new_spf = (uint32_t*)calloc(newlen, sizeof(uint32_t));
            if (new_spf == NULL) abort();
            for (idx_t i = 0; i <= limit; ++i) new_spf[i] = (uint32_t)i;
            for (idx_t i = 2; i * i <= limit; ++i) {
                if (new_spf[i] == (uint32_t)i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
                }
            }
            spf = new_spf;
            spf_len = newlen;
        } else {
            new_spf = (uint32_t*)realloc(spf, newlen * sizeof(uint32_t));
            if (new_spf == NULL) abort();
            spf = new_spf;
            /* set new entries initial value = index */
            for (idx_t i = spf_len; i <= limit; ++i) spf[i] = (uint32_t)i;
            /* sieve new range */
            for (idx_t i = 2; i * i <= limit; ++i) {
                if (spf[i] == (uint32_t)i) {
                    idx_t start = i * i;
                    if (start < spf_len) start = ((spf_len + i - 1) / i) * i;
                    for (idx_t j = start; j <= limit; j += i)
                        if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
            spf_len = newlen;
        }
    }

    /* Ensure prefix and omega arrays cover [0, limit]. */
    if (prefix_len <= limit) {
        size_t oldlen = (prefix_len != 0) ? (size_t)prefix_len : 3; /* at least 3 to start */
        size_t newlen = (size_t)(limit + 1);
        seq_t *new_prefix;
        uint8_t *new_omega;
        if (prefix == NULL) {
            new_prefix = (seq_t*)calloc(newlen, sizeof(seq_t));
            new_omega  = (uint8_t*)calloc(newlen, sizeof(uint8_t));
            if (new_prefix == NULL || new_omega == NULL) abort();
            /* indices 0 and 1 are already zero-by-calloc */
            for (idx_t i = 2; i <= limit; ++i) {
                uint32_t sp = spf[i];
                new_omega[i] = new_omega[i / sp] + 1;
                new_prefix[i] = new_prefix[i - 1] + (seq_t)new_omega[i];
            }
            prefix = new_prefix;
            omega  = new_omega;
            prefix_len = newlen;
        } else {
            new_prefix = (seq_t*)realloc(prefix, newlen * sizeof(seq_t));
            new_omega  = (uint8_t*)realloc(omega, newlen * sizeof(uint8_t));
            if (new_prefix == NULL || new_omega == NULL) abort();
            for (idx_t i = oldlen; i <= limit; ++i) {
                uint32_t sp = spf[i];
                new_omega[i] = new_omega[i / sp] + 1;
                new_prefix[i] = new_prefix[i - 1] + (seq_t)new_omega[i];
            }
            prefix = new_prefix;
            omega  = new_omega;
            prefix_len = newlen;
        }
    }
}

/* Compute the n-th term of OEIS sequence A023823. */
seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 safely. */
    if (n == 0) return 0;
    idx_t limit = 3 * n;
    _ensure_cache(limit);
    seq_t res = prefix[3 * n] - prefix[n + 1] - prefix[2 * n - 1];
    return res;
}
