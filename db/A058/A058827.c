/*
 * A058827  Totients which are not cototients.
 * https://oeis.org/A058827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer that occurs as a totient *
 *            (φ(k) for some k) and does NOT occur as a cototient (m – φ(m)
 *            for any m). * The function builds a sieve of φ and of m‑φ(m) up
 *            to a growing limit * until it has enough terms to answer the
 *            requested index
 * Range:     u8 n <= 9, u16 n <= 1987, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*      static data                                                    */
/* ------------------------------------------------------------------ */
static int          built          = 0;          /* 0 = not built yet          */
static size_t       cur_limit      = 0;          /* current sieve limit        */
static size_t       terms_len      = 0;          /* number of terms found      */
static size_t       terms_cap      = 0;          /* capacity of terms array    */

static uint8_t    *is_comp         = NULL;       /* composite flags            */
static uint32_t   *phi             = NULL;       /* φ values                    */
static uint8_t    *tot_present     = NULL;       /* is x a totient?            */
static uint8_t    *cot_present     = NULL;       /* is x a cototient?          */
static int        *primes          = NULL;       /* prime list                 */
static seq_t      *terms           = NULL;       /* collected terms            */

/* ------------------------------------------------------------------ */
/*      helper: build the tables for a given limit                    */
/* ------------------------------------------------------------------ */
static void build_tables(size_t limit)
{
    /* free old data if any */
    free(is_comp);
    free(phi);
    free(tot_present);
    free(cot_present);
    free(primes);
    free(terms);

    cur_limit   = limit;
    terms_len   = 0;
    terms_cap   = 128;

    is_comp     = (uint8_t*)calloc(limit + 1, 1);
    phi         = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    tot_present = (uint8_t*)calloc(limit + 1, 1);
    cot_present = (uint8_t*)calloc(limit + 1, 1);
    primes      = (int*)malloc((limit / 10 + 1) * sizeof(int));   /* rough estimate */
    terms       = (seq_t*)malloc(terms_cap * sizeof(seq_t));

    if (!is_comp || !phi || !tot_present || !cot_present || !primes || !terms)
        abort();   /* out of memory */

    /* linear sieve for φ up to limit */
    phi[1] = 1;
    size_t pc = 0;   /* number of primes found */

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            phi[i] = (uint32_t)(i - 1);
        }
        for (size_t j = 0; j < pc; ++j) {
            size_t ip = i * (size_t)primes[j];
            if (ip > limit) break;
            is_comp[ip] = 1;
            if (i % primes[j] == 0) {
                phi[ip] = phi[i] * (uint32_t)primes[j];
                break;
            } else {
                phi[ip] = phi[i] * (uint32_t)(primes[j] - 1);
            }
        }
    }

    /* mark totients and cototients within the limit */
    for (size_t i = 2; i <= limit; ++i) {
        uint32_t vphi = phi[i];
        if (vphi <= limit)
            tot_present[vphi] = 1;
        uint32_t val = (uint32_t)(i - vphi);
        if (val >= 1 && (size_t)val <= limit)
            cot_present[val] = 1;
    }

    /* collect terms that are totients but not cototients */
    for (size_t i = 1; i <= limit; ++i) {
        if (tot_present[i] && !cot_present[i]) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)i;
        }
    }

    built = 1;
}

/* ------------------------------------------------------------------ */
/*      ensure we have at least N terms                               */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    while (!built || n > terms_len) {
        size_t new_lim = (built ? cur_limit << 1 : 2000000);  /* start at 2e6 */
        build_tables(new_lim);
    }
}

/* ------------------------------------------------------------------ */
/*      sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence is defined for n >= 1 */
    ensure_terms(n);
    return terms[n - 1];
}
