/*
 * A097453  Primes in A014237 in the order of their appearance.
 * https://oeis.org/A097453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime‑difference prime. For k ≥ 1 let p(k) be
 *            the k‑th prime and c(k) the k‑th composite (1 is treated as the
 *            first composite). The integer d(k)=p(k)−c(k) is computed; a(n)
 *            lists those d(k) that are prime, in the order in which they
 *            appear. The function returns the value of a(n) f...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* --------- static cache --------- */
static bool initialized = false;
static seq_t *primes = NULL;
static seq_t *composites = NULL;
static size_t prime_cnt = 0;
static size_t composite_cnt = 0;

static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_cnt = 0;
static size_t next_k = 0;          /* next index k to process */

/* --------- helper: grow terms array --------- */
static void ensure_terms_cap(size_t needed)
{
    if (terms == NULL) {
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else {
        while (terms_cap < needed) {
            terms_cap <<= 1;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
    }
}

/* --------- public function --------- */
seq_t a(idx_t n)
{
    /* ----- initialise primes and composites ----- */
    if (!initialized) {
        /* A generous bound that covers many thousands of terms. */
        const idx_t LIMIT = 500000;

        /* -------- sieve of Eratosthenes -------- */
        bool *is_prime = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
        if (!is_prime) exit(EXIT_FAILURE);
        for (idx_t i = 0; i <= LIMIT; ++i) is_prime[(size_t)i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (!is_prime[(size_t)p]) continue;
            for (idx_t k = p * p; k <= LIMIT; k += p)
                is_prime[(size_t)k] = false;
        }

        /* ---- count primes ---- */
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (is_prime[(size_t)i]) ++prime_cnt;

        /* ---- allocate prime array ---- */
        primes = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
        if (!primes) { free(is_prime); exit(EXIT_FAILURE); }
        size_t pi = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (is_prime[(size_t)i])
                primes[pi++] = (seq_t)i;

        /* ---- composite array: 1 followed by all non‑prime numbers ---- */
        composite_cnt = LIMIT - prime_cnt + 1;   /* include 1 */
        composites = (seq_t *)malloc(composite_cnt * sizeof(seq_t));
        if (!composites) { free(is_prime); free(primes); exit(EXIT_FAILURE); }
        composites[0] = (seq_t)1;
        size_t ci = 1;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (!is_prime[(size_t)i])
                composites[ci++] = (seq_t)i;

        free(is_prime);
        initialized = true;
    }

    /* ----- generate terms until we have at least n of them ----- */
    while (terms_cnt < (size_t)n) {
        if (next_k >= prime_cnt || next_k >= composite_cnt) {
            /* The pre‑computed range was too small – should not happen. */
            exit(EXIT_FAILURE);
        }

        seq_t prime_val     = primes[next_k];
        seq_t composite_val = composites[next_k];

        if (prime_val > composite_val) {
            seq_t diff = prime_val - composite_val;
            if (diff > 1 && is_prime_small(diff)) {
                ensure_terms_cap(terms_cnt + 1);
                terms[terms_cnt++] = diff;
            }
        }
        ++next_k;
    }

    return terms[n - 1];
}
