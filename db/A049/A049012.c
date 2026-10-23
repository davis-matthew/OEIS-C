/*
 * A049012  Composite numbers n such that number of nonprime d with 0 < d < n,
 *          gcd(n,d)=1, is pi(n).
 * https://oeis.org/A049012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049012:
   a(n) is the n‑th composite number n for which
   phi(n) + ω(n) = 2·π(n)
   where φ is Euler’s totient, ω is the number of distinct prime factors
   and π counts primes ≤ n. */

/* The sequence is finite; all its terms are < 10 000 000.
   The routine precomputes the full list once and then returns it
   by index.  Subsequent calls are O(1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* global storage for the precomputed list */
static seq_t *g_terms = NULL;
static size_t g_terms_len = 0;

/* ------------------------------------------------------------------ */
/* helper that builds the full list of terms up to N_MAX.             */
/* ------------------------------------------------------------------ */
static void build_sequence(void)
{
    const size_t N_MAX = 10000000;          /* 10^7  */
    /* allocate arrays for sieve */
    bool   *is_comp   = (bool*)    malloc((N_MAX + 1) * sizeof(bool));
    uint32_t *phi     = (uint32_t*)malloc((N_MAX + 1) * sizeof(uint32_t));
    uint8_t  *omega   = (uint8_t*) malloc((N_MAX + 1) * sizeof(uint8_t));
    uint32_t *primes  = (uint32_t*)malloc((N_MAX / 10 + 10) * sizeof(uint32_t));

    if (!is_comp || !phi || !omega || !primes)
        return; /* allocation failure – caller will get empty list */

    size_t prime_cnt = 0;
    size_t primes_capacity = N_MAX / 10 + 10;

    /* initialise */
    is_comp[0] = is_comp[1] = false;
    phi[0] = 0; phi[1] = 1;
    omega[0] = 0; omega[1] = 0;

    /* linear sieve for phi and omega */
    for (size_t i = 2; i <= N_MAX; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < primes_capacity) {
                primes[prime_cnt] = (uint32_t)i;
                ++prime_cnt;
            }
            phi[i] = (uint32_t)i - 1;
            omega[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            size_t ip = i * (size_t)p;
            if (ip > N_MAX) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                phi[ip] = phi[i] * (uint32_t)p;
                omega[ip] = omega[i];
                break;
            } else {
                phi[ip] = phi[i] * ((uint32_t)p - 1);
                omega[ip] = omega[i] + 1;
            }
        }
    }

    /* collect terms */
    size_t terms_cap = 1024;
    seq_t *terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) {
        free(is_comp); free(phi); free(omega); free(primes);
        return;
    }
    size_t terms_len = 0;

    size_t pi = 0;               /* running π(n) */
    for (size_t n = 2; n <= N_MAX; ++n) {
        if (!is_comp[n]) {  /* prime */
            ++pi;
            continue;
        }
        uint64_t left  = (uint64_t)phi[n] + (uint64_t)omega[n];
        uint64_t right = (uint64_t)2 * pi;
        if (left == right) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) break; /* out of memory */
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)n;
        }
    }

    /* free temporary arrays */
    free(is_comp); free(phi); free(omega); free(primes);

    /* replace global list with the new one */
    free(g_terms);
    g_terms = terms;
    g_terms_len = terms_len;
}

/* ------------------------------------------------------------------ */
/* public routine requested by the exercise                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_sequence();
        initialized = true;
    }

    if (n == 0 || n > g_terms_len)
        return (seq_t)0;     /* out of range – the sequence is finite */

    return g_terms[n - 1];
}
