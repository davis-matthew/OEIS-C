/*
 * A093317  Consider numbers n such that mu(n) = mu(n+1), A064148; sequence
 *          gives values of mu(A064148(n)).
 * https://oeis.org/A093317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A093317: a(n) = μ( A064148(n) ),
    where A064148(n) is the n‑th integer k such that μ(k) = μ(k+1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*  External types supplied by the build; do NOT redefine them.  */
 /*  seq_t : the sequence value type                               */
 /*  idx_t : the index type (unsigned, 64‑bit)                     */

static int8_t   *mu          = NULL;   /* Möbius values, 1‑based indexed   */
static size_t   mu_limit    = 0;      /* size of μ array                  */
static uint32_t *primes      = NULL;   /* list of primes used              */
static size_t   prime_cnt   = 0;      /* number of primes found           */

static seq_t   *seq_terms   = NULL;   /* computed sequence terms          */
static size_t   terms_len   = 0;      /* number of terms stored           */
static size_t   terms_cap   = 0;      /* capacity of seq_terms array      */

static idx_t    search_pos  = 1;      /* next integer to test for µ(k)=µ(k+1) */

/*  Recompute the Möbius sieve up to 'limit' (inclusive).              */
static void sieve_mu(idx_t limit)
{
    int8_t   *new_mu = (int8_t*)malloc((limit + 1) * sizeof(int8_t));
    bool     *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    uint32_t *new_pr = (uint32_t*)malloc(((limit / 10) + 10) * sizeof(uint32_t));

    if (!new_mu || !is_comp || !new_pr) {
        free(new_mu); free(is_comp); free(new_pr);
        exit(1);
    }

    new_mu[0] = 0;
    if (limit >= 1) new_mu[1] = 1;

    size_t   pcnt    = 0;
    size_t   prime_cap = ((limit / 10) + 10);

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (pcnt >= prime_cap) {                 /* grow prime buffer */
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(new_pr, prime_cap * sizeof(uint32_t));
                if (!tmp) {
                    free(new_mu); free(is_comp); free(new_pr);
                    exit(1);
                }
                new_pr = tmp;
            }
            new_pr[pcnt++] = (uint32_t)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = new_pr[j];
            idx_t prod = i * p;
            if (prod > limit) break;
            is_comp[prod] = true;
            if (i % p == 0) {
                new_mu[prod] = 0;
                break;
            } else {
                new_mu[prod] = -new_mu[i];
            }
        }
    }

    /* Replace old tables with the freshly computed ones. */
    free(mu);
    free(primes);
    free(is_comp);

    mu      = new_mu;
    primes  = new_pr;
    mu_limit = limit;
    prime_cnt = pcnt;
}

/*  Append a value to the dynamic sequence array.                     */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        seq_terms = (seq_t*)realloc(seq_terms, terms_cap * sizeof(seq_t));
        if (!seq_terms) exit(1);
    }
    seq_terms[terms_len++] = val;
}

/*  Make sure at least 'target' terms are available.                  */
static void ensure_terms(size_t target)
{
    if (terms_len >= target) return;

    /* Ensure μ table covers at least search_pos + 1.                 */
    idx_t need = search_pos + 1;
    if (need > mu_limit) {
        idx_t new_limit = mu_limit ? mu_limit * 2 : 2000000;
        while (new_limit < need) new_limit <<= 1;
        sieve_mu(new_limit);
    }

    while (terms_len < target) {
        /* Find next k with μ(k)=μ(k+1). */
        while (true) {
            if (search_pos + 1 > mu_limit) {
                idx_t new_limit = mu_limit ? mu_limit * 2 : 2000000;
                while (new_limit < (idx_t)(search_pos + 1)) new_limit <<= 1;
                sieve_mu(new_limit);
            }
            if (mu[search_pos] == mu[search_pos + 1]) {
                append_term((seq_t)mu[search_pos]);
                ++search_pos;               /* skip next position to avoid double‑counting */
                break;
            }
            ++search_pos;
        }
    }
}

/*  The required sequence function.                                  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset; harmless fallback */
    ensure_terms((size_t)n);
    return seq_terms[(size_t)n - 1];
}
