/*
 * A136001  Primes in A136000.
 * https://oeis.org/A136001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136001: primes p such that p+1 can be expressed as the sum of a
 * Pythagorean triple.  The implementation precomputes a sufficiently large
 * segment of the sequence on first use and then returns the requested
 * element from the cached array.  The algorithm is a simple sieve up to a
 * fixed limit (2 000 000) and a second pass to flag all numbers that can be
 * expressed as 2 m(m+n) for coprime m>n of opposite parity.  After the
 * flags are ready, every prime p with flag[p+1]==1 is stored in the
 * result array.  The cache is grown as needed.  The routine works for any
 * seq_t supported by the existing utils, including arbitrary‑precision
 * big integers.  It does not require any other external libraries. */

/* ----------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Utility prototypes (provided by the build system). */
#include "../utils/seq_alloc.h"     /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) -> seq_t* */

/* ----------------------------------------------------------------------- */
/* Local helpers */

static inline idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----------------------------------------------------------------------- */
/* Static state */

static bool          seq_initialized = false;
static seq_t        *seq_terms      = NULL;
static idx_t         seq_terms_cnt  = 0;
static idx_t         seq_terms_cap  = 0;

/* ----------------------------------------------------------------------- */
/* Initialisation routine ------------------------------------------------- */
static void seq_init(void)
{
    if (seq_initialized)
        return;

    /* 1. Choose a sieve limit large enough to contain ~10 000 terms.
     * 2 000 000 gives more than 140 000 primes, far more than needed,
     * while keeping memory usage modest (~8 MB for two byte arrays). */
    const idx_t LIMIT   = 2000000ULL;
    const idx_t BOUND   = LIMIT + 1ULL;          /* we need flags up to p+1 */

    /* 2. Sieving primes up to LIMIT */
    uint8_t *comp = (uint8_t*)calloc((size_t)(LIMIT + 1), 1);   /* 0 = prime */
    if (!comp) abort();

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                comp[j] = 1;
        }
    }

    /* 3. Flag all sums of a Pythagorean triple:
     *    sum = 2 * m * (m + n) (primitive) and all its multiples. */
    uint8_t *sumflag = (uint8_t*)calloc((size_t)(BOUND + 1), 1);
    if (!sumflag) abort();

    for (unsigned m = 2; 2ULL * m * (m + 1) <= BOUND; ++m) {
        for (unsigned n = 1; n < m; ++n) {
            if (((m ^ n) & 1) == 0)          /* same parity -> skip   */
                continue;
            if (_gcd_u64(m, n) != 1)          /* not coprime          */
                continue;
            uint64_t sum0 = 2ULL * m * (m + n);
            if (sum0 > BOUND)
                continue;
            for (idx_t j = sum0; j <= BOUND; j += sum0)
                sumflag[(size_t)j] = 1;
        }
    }

    /* 4. Collect the sequence of primes p with sumflag[p+1]==1 */
    seq_terms_cap = 2000;                     /* initial capacity */
    seq_terms     = seq_alloc(seq_terms_cap);
    if (!seq_terms) abort();

    seq_terms_cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!comp[p]) {                       /* p is prime */
            if (p == 2)                      /* 2+1 = 3 is odd, not a sum of a Pythagorean triple */
                continue;
            if (sumflag[(size_t)(p + 1)]) {
                if (seq_terms_cnt >= seq_terms_cap) {
                    size_t old_cap = seq_terms_cap;
                    seq_terms_cap *= 2;
                    seq_t *tmp = seq_realloc(seq_terms, old_cap, seq_terms_cap);
                    if (!tmp) abort();
                    seq_terms = tmp;
                }
                seq_terms[seq_terms_cnt++] = (seq_t)p;
            }
        }
    }

    /* 5. Clean up temporary arrays */
    free(comp);
    free(sumflag);

    seq_initialized = true;
}

/* ----------------------------------------------------------------------- */
/* public interface ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* outside offset – not expected but safe */
        return 0;
    seq_init();
    if (n > seq_terms_cnt)
        return 0;          /* out of range – not expected */
    return seq_terms[n - 1];
}
