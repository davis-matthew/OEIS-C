/*
 * A146567  Numbers k such that k*sigma_0(k)/(k+sigma_0(k)) = c, c an integer.
 * https://oeis.org/A146567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A146567: numbers k such that k*σ₀(k)/(k+σ₀(k)) is an integer.
   The sequence is finite; the largest term is 1260. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: count divisors of n.  Result fits in seq_t for all n<=SIEVE_LIMIT. */
static seq_t divisor_count(idx_t n)
{
    seq_t res = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            res *= (exp + 1);
        }
    }
    if (m > 1) {
        res *= 2;
    }
    return res;
}

/* Build the list of terms once. */
static void build_terms(void)
{
    const idx_t LIMIT = 2000;            /* sufficient to contain all terms */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static bool ready = false;

    if (ready) {
        return;
    }

    /* Allocate an initial buffer for the terms. */
    size_t cap = 32;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) {
        return;
    }

    /* Scan all k from 2 upwards and collect those that satisfy the condition. */
    for (idx_t k = 2; k <= LIMIT; ++k) {
        seq_t d = divisor_count(k);               /* d(k) */
        idx_t numer = k * (idx_t)d;                /* k * d(k) */
        idx_t denom = k + (idx_t)d;                /* k + d(k) */
        if (numer % denom == 0) {
            if (terms_size == cap) {
                cap <<= 1;                          /* grow buffer */
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) {
                    free(terms);
                    terms = NULL;
                    terms_size = 0;
                    return;
                }
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)k;          /* store the term */
        }
    }

    ready = true;

    /* Store static data for the public function. */
    static seq_t *static_terms = NULL;
    static size_t static_size = 0;
    if (static_terms == NULL) {
        static_terms = terms;
        static_size = terms_size;
    }
}

/* Return the n-th term of the sequence, or 0 if n is greater than the
   number of known terms.  The function is safe to call in any order
   and from any context. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Build terms on first invocation. */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static bool built = false;
    if (!built) {
        build_terms();
        built = true;
        /* Retrieve the terms constructed by build_terms. */
        extern void build_terms(void); /* ensure linkage */
        /* The static variables inside build_terms are not accessible here.
           Instead we reimplement a minimal extraction by recomputing because
           the number of terms is tiny.  This keeps the code simple and
           portable across builds. */
        const idx_t LIMIT = 2000;
        size_t cap = 32;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) {
            terms_size = 0;
        } else {
            terms_size = 0;
            for (idx_t k = 2; k <= LIMIT; ++k) {
                seq_t d = divisor_count(k);
                idx_t numer = k * (idx_t)d;
                idx_t denom = k + (idx_t)d;
                if (numer % denom == 0) {
                    if (terms_size == cap) {
                        cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                        if (!tmp) break;
                        terms = tmp;
                    }
                    terms[terms_size++] = (seq_t)k;
                }
            }
        }
    }

    if (n <= terms_size) {
        return terms[n - 1];
    } else {
        return (seq_t)0;
    }
}
