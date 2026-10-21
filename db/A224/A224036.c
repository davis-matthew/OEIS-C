/*
 * A224036  Number of nX6 0..1 arrays with antidiagonals unimodal and rows and
 *          diagonals nondecreasing.
 * https://oeis.org/A224036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (valid for n > 13):
   a(n) = 7*a(n-1) -15*a(n-2) +6*a(n-3) +11*a(n-4)
          -4*a(n-5) -7*a(n-6) +6*a(n-7) -6*a(n-8)
          +8*a(n-9) -8*a(n-10) +16*a(n-11)
   Initial terms (n = 1 … 13):
   7, 34, 119, 363, 1064, 3107, 9125, 26928, 79800, 237348,
   708163, 2118654, 6353180 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage, types are supplied by the build system */
extern seq_t a(idx_t n);

/* Static cache for memoization */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;   /* allocated size of CACHE   */
static size_t CACHE_LEN = 0;   /* number of terms already computed (highest n) */

/* First 13 terms of the sequence */
static const seq_t INIT[13] = {
    (seq_t)7,      (seq_t)34,     (seq_t)119,    (seq_t)363,
    (seq_t)1064,   (seq_t)3107,    (seq_t)9125,   (seq_t)26928,
    (seq_t)79800,  (seq_t)237348, (seq_t)708163, (seq_t)2118654,
    (seq_t)6353180
};

seq_t a(idx_t n) {
    /* n is 1‑based; a(1) is the first term. */
    if (n == 0) return (seq_t)0;   /* Not used, but safe. */

    /* Initialise cache on first call. */
    if (CACHE == NULL) {
        CACHE_CAP = 20;                 /* initial allocation size */
        CACHE = (seq_t *)calloc(CACHE_CAP, sizeof(seq_t));
        if (!CACHE) return (seq_t)0;    /* out‑of‑memory failure */

        for (size_t i = 0; i < 13; ++i)
            CACHE[i + 1] = INIT[i];    /* a(1) … a(13) */
        CACHE_LEN = 13;
    }

    /* If requested value already cached, return it. */
    if ((size_t)n <= CACHE_LEN) return CACHE[(size_t)n];

    /* Grow cache if needed. */
    if ((size_t)n >= CACHE_CAP) {
        size_t new_cap = CACHE_CAP;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out‑of‑memory failure */
        CACHE = tmp;
        CACHE_CAP = new_cap;
    }

    /* Compute terms from CACHE_LEN+1 up to n. */
    for (size_t i = CACHE_LEN + 1; i <= (size_t)n; ++i) {
        if (i <= 13) {
            /* Already filled during initialisation, but keep it here
               for completeness when COMPILING with COUNTED indices. */
            continue;
        }
        /* recurrence:
           a(n) = 7*a(n-1) -15*a(n-2) +6*a(n-3) +11*a(n-4)
                  -4*a(n-5) -7*a(n-6) +6*a(n-7) -6*a(n-8)
                  +8*a(n-9) -8*a(n-10) +16*a(n-11) */
        CACHE[i] = (seq_t)7 * CACHE[i-1]
                 - (seq_t)15 * CACHE[i-2]
                 + (seq_t)6  * CACHE[i-3]
                 + (seq_t)11 * CACHE[i-4]
                 - (seq_t)4  * CACHE[i-5]
                 - (seq_t)7  * CACHE[i-6]
                 + (seq_t)6  * CACHE[i-7]
                 - (seq_t)6  * CACHE[i-8]
                 + (seq_t)8  * CACHE[i-9]
                 - (seq_t)8  * CACHE[i-10]
                 + (seq_t)16 * CACHE[i-11];
    }

    CACHE_LEN = (size_t)n;
    return CACHE[(size_t)n];
}
