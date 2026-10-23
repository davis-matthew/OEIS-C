/*
 * A018889  Numbers whose shortest representation as a sum of positive cubes
 *          requires exactly 8 cubes.
 * https://oeis.org/A018889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018889: Numbers whose shortest representation as a sum of positive cubes requires exactly 8 cubes.
   The implementation uses dynamic programming to compute the minimal number of cubes needed for each integer
   and collects the numbers that need exactly 8 cubes. */

#include <stdlib.h>
#include <stdint.h>

/*  dp_counts[i] contains the minimal number of positive cubes needed to represent i;
    values larger than 255 are treated as “unreachable” (INF). */
static unsigned char *dp_counts = NULL;
static size_t          dp_max     = 0;   /* maximum integer for which dp_counts has been filled */

/*  cubes[k] = (k+1)^3  for k = 0 .. cubes_len-1  */
static uint64_t *cubes     = NULL;
static size_t    cubes_len = 0;

/*  terms holds the sequence elements (integers that require exactly 8 cubes). */
static seq_t  *terms      = NULL;
static size_t  terms_cnt  = 0;
static size_t  terms_cap  = 0;

/*  Ensure dp_counts and cubes cover all integers up to new_max.  Then
    fill dp_counts for the new range, and record terms that use exactly
    8 cubes. */
static void enlarge(size_t new_max) {
    if (new_max <= dp_max) return;

    /* Resize dp_counts array.  Uses realloc; first time allocate from scratch. */
    dp_counts = (unsigned char *)realloc(dp_counts, (new_max + 1) * sizeof(unsigned char));

    /* Pre‑compute all cubes <= new_max. */
    size_t cube_root = 0;
    while (((uint64_t)(cube_root + 1)) * (cube_root + 1) * (cube_root + 1) <= new_max)
        ++cube_root;

    if (cube_root + 1 > cubes_len) {
        cubes = (uint64_t *)realloc(cubes, (cube_root + 1) * sizeof(uint64_t));
        for (size_t j = cubes_len; j <= cube_root; ++j) {
            uint64_t v = (uint64_t)(j + 1);
            cubes[j] = v * v * v;
        }
        cubes_len = cube_root + 1;
    }

    /* Compute dp_counts from dp_max+1 up to new_max. */
    for (size_t i = dp_max + 1; i <= new_max; ++i) {
        unsigned char best = 255; /* INF */
        for (size_t j = 0; j < cubes_len && cubes[j] <= i; ++j) {
            size_t idx = i - (size_t)cubes[j];
            unsigned char prev = dp_counts[idx];
            if (prev == 255) continue;
            unsigned char cand = prev + 1;
            if (cand < best) best = cand;
        }
        dp_counts[i] = best;

        if (best == 8) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 1024;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)i;
        }
    }

    dp_max = new_max;
}

/*  Make sure at least n terms are available.  The function grows the
    search range until enough terms have been found. */
static void ensure_terms(idx_t n) {
    if (!dp_counts) { /* first call – initialise static storage */
        dp_counts = (unsigned char *)malloc(1);
        dp_counts[0] = 0;
        dp_max     = 0;
        cubes      = NULL;
        cubes_len  = 0;
        terms      = NULL;
        terms_cap  = 0;
    }

    while (terms_cnt < (size_t)n) {
        size_t new_max = dp_max ? dp_max * 2 + 1000 : 1000;
        enlarge(new_max);
        /* If enlarge fails to add more terms because density is low,
           the loop will simply request a larger range next iteration. */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n = 0 */
    ensure_terms(n);
    return terms[n - 1];
}
