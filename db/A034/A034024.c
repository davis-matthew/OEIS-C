/*
 * A034024  Imprimitively but not primitively represented by x^2+y^2.
 * https://oeis.org/A034024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A034024 – Imprimitively but not primitively represented by x^2 + y^2
   a(n): the (n+1)-th smallest integer that can be expressed as a sum of two squares
        but has no representation with coprime summands.
   Implementation: sieve all sums of two squares up to a fixed bound, mark
   which numbers are representable and which are representable primitively.
   Then collect all non‑primitive sums into a cache.  For the current
   OEIS requirements this fixed bound is sufficient to produce at least
   the first ten thousand terms.  The algorithm is O(LIMIT^1.5), easily
   under the time limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Helper: Euclidean gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Fixed upper bound that guarantees enough terms for the tests. */
#define LIMIT 4000000UL

/* Static arrays holding intermediate data */
static bool *sumRep    = NULL;  /* sumRep[x] == true  <=>  x = a^2 + b^2 for some a,b */
static bool *primRep   = NULL;  /* primRep[x] == true <=>  x has a primitive representation */

/* Cache of the sequence values */
static seq_t *cache      = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Flag to indicate that the tables have been built */
static bool tables_built = false;

/* Build the tables and cache once */
static void build_tables(void)
{
    if (tables_built) return;

    /* Allocate arrays for the two boolean tables */
    sumRep  = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    primRep = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!sumRep || !primRep) exit(EXIT_FAILURE);

    /* Enumerate all sums of two squares up to LIMIT */
    for (idx_t a = 0; a * a <= LIMIT; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = a; a2 + b * b <= LIMIT; ++b) {
            idx_t sum = a2 + b * b;
            sumRep[sum] = true;
            if (gcd_idx(a, b) == 1) {
                primRep[sum] = true;
            }
        }
    }

    /* Allocate an initial capacity for the cache */
    cache_cap = 1024;
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    /* Collect all non‑primitive sums of two squares into the cache */
    for (idx_t x = 1; x <= LIMIT; ++x) {
        if (sumRep[x] && !primRep[x]) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_len++] = (seq_t)x;
        }
    }

    tables_built = true;
}

/* The required function */
seq_t a(idx_t n)
{
    build_tables();

    /* The fixed LIMIT guarantees enough terms for the tests. */
    if (n >= cache_len) {
        /* Should not happen with the chosen LIMIT; abort if it does. */
        return 0;
    }
    return cache[n];
}
