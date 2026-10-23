/*
 * A015851  Numbers k such that phi(k) | sigma(k + 8).
 * https://oeis.org/A015851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that φ(k) divides σ(k+8)
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *phi = NULL;          /* φ values  (size: limit+9) */
static seq_t *sigma = NULL;        /* σ values (size: limit+9) */
static seq_t *terms = NULL;       /* list of found k's                  */
static size_t terms_cap = 0;       /* allocated capacity of terms        */
static size_t terms_len = 0;       /* current number of terms found      */
static idx_t current_limit = 0;    /* current maximum k searched         */

/* append a term to the dynamic array */
static void add_term(seq_t v) {
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap << 1U : 16U;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

/* recompute φ and σ up to new_limit and rebuild the term list */
static void recompute(idx_t new_limit) {
    /* allocate arrays large enough for φ up to new_limit and σ up to new_limit+8 */
    idx_t size = new_limit + 9;          /* index 0 … new_limit+8 inclusive */
    phi   = (seq_t *)realloc(phi,   size * sizeof(seq_t));
    sigma = (seq_t *)realloc(sigma, size * sizeof(seq_t));

    /* initialise φ and σ */
    for (idx_t i = 0; i < size; ++i) {
        phi[i]   = (seq_t)i;    /* φ[i] = i   */
        sigma[i] = 0;           /* σ[i] = 0   */
    }

    /* compute φ using a simple sieve */
    for (idx_t p = 2; p <= new_limit; ++p) {
        if (phi[p] == (seq_t)p) {          /* p is prime */
            for (idx_t j = p; j <= new_limit; j += p) {
                phi[j] -= phi[j] / (seq_t)p;
            }
        }
    }

    /* compute σ by summing divisors */
    idx_t sigma_limit = new_limit + 8;
    for (idx_t i = 1; i <= sigma_limit; ++i) {
        for (idx_t j = i; j <= sigma_limit; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* rebuild the list of terms up to new_limit */
    terms_len = 0;
    if (terms_cap < 16) {
        terms_cap = 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    for (idx_t k = 1; k <= new_limit; ++k) {
        if ( (sigma[k + 8] % phi[k]) == 0 ) {
            add_term((seq_t)k);
        }
    }

    current_limit = new_limit;
}

/* public function required by the build environment */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1 – ignore n==0 */
    /* start with a modest limit; enlarge as needed */
    if (current_limit == 0) recompute(8192U);
    /* until we have enough terms, double the search range */
    while (terms_len < (size_t)n) {
        recompute(current_limit * 2U);
    }
    return terms[n - 1];
}
