/*
 * A054832  Fifth term of weak prime sextet: p(m-3)-p(m-4) < p(m-2)-p(m-3) <
 *          p(m-1)-p(m-2) < p(m)-p(m-1) < p(m+1)-p(m).
 * https://oeis.org/A054832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fifth term of weak prime sextet  
   a(n) = the 5th prime of a 6‑prime window  
           [p_i , p_{i+1} , p_{i+2} , p_{i+3} , p_{i+4} , p_{i+5}]  
           such that 
           p_{i+1}-p_i  <  p_{i+2}-p_{i+1}  <  p_{i+3}-p_{i+2}  
           < p_{i+4}-p_{i+3}  <  p_{i+5}-p_{i+4}. */

/* For the first 24 terms:
   2939 13499 13921 14983 15401 15413 21433 21577 21893 28297
   30911 33247 35617 37747 42257 42611 45841 55681 64693 64951
   64969 68227 68239 68917 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "../utils/build_primes.h"

/* ==== Static state for memoisation ==== */
static seq_t *SEQ_CACHE = NULL;   /* array of found terms          */
static size_t SEQ_LEN   = 0;      /* number of terms stored        */

static uint64_t LIMIT = 30000000ULL;   /* current sieve upper bound    */

/* append a term to the cache (guaranteed large enough) */
static void append_term(seq_t val)
{
    if (SEQ_LEN == 0) {
        SEQ_CACHE = (seq_t*)malloc(16 * sizeof(seq_t));
    } else if (SEQ_LEN % 16 == 0) {
        SEQ_CACHE = (seq_t*)realloc(SEQ_CACHE,
                                    (SEQ_LEN + 16) * sizeof(seq_t));
    }
    SEQ_CACHE[SEQ_LEN++] = val;
}

/* generate all terms up to a given sieve limit */
static void generate_up_to_limit(void)
{
    /* 1. build prime list up to LIMIT  */
    idx_t *primes = NULL;
    size_t pcount = 0;
    build_primes((idx_t)LIMIT, &primes, &pcount);

    /* 2. free old cache (rebuild from scratch) */
    free(SEQ_CACHE);
    SEQ_CACHE = NULL;
    SEQ_LEN   = 0;

    /* 3. scan consecutive windows of six primes   */
    for (size_t i = 0; i + 5 < pcount; ++i) {
        /* compute successive gaps */
        idx_t g1 = primes[i + 1] - primes[i];
        idx_t g2 = primes[i + 2] - primes[i + 1];
        idx_t g3 = primes[i + 3] - primes[i + 2];
        idx_t g4 = primes[i + 4] - primes[i + 3];
        idx_t g5 = primes[i + 5] - primes[i + 4];

        if (g1 < g2 && g2 < g3 && g3 < g4 && g4 < g5) {
            /* the 5th term of the sextet is primes[i+4] */
            append_term((seq_t)primes[i + 4]);
        }
    }

    free(primes);
}

/* ensure enough terms are available for a given n */
static void ensure_terms(idx_t n)
{
    while (SEQ_LEN < (size_t)n) {
        generate_up_to_limit();
        if (SEQ_LEN < (size_t)n) {
            LIMIT *= 2;          /* increase range and rebuild */
        }
    }
}

/* ==== Public function ==== */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;    /* only defined for n ≥ 1 */
    ensure_terms(n);
    return SEQ_CACHE[n - 1];
}
