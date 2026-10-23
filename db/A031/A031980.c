/*
 * A031980  a(n) is the smallest number >= 1 not occurring earlier and not the
 *          sum of cubes of two distinct earlier terms.
 * https://oeis.org/A031980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_pair_exists, is_sum_of_cubes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031980:
 *
 * a(n) is the smallest integer ≥ 1 that has not appeared earlier
 * and is not expressible as the sum of the cubes of two distinct
 * earlier terms. The implementation constructs the sequence
 * incrementally, memoising all results so that subsequent calls are
 * efficient, independent of the order in which n is requested.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/cube_pair_exists.h"
#include "../utils/is_sum_of_cubes.h"

/*--- Helper: grow the used array to at least 'needed' entries --------------*/
static void ensure_used_cap(size_t needed,
                            uint8_t **used,
                            size_t *used_cap)
{
    if (needed < *used_cap)
        return;

    size_t new_cap = (*used_cap) ? (*used_cap * 2) : 16;
    while (needed >= new_cap)
        new_cap <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(*used, new_cap);
    if (!tmp)
        abort();
    if (new_cap > *used_cap) {
        /* zero new entries */
        memset(tmp + *used_cap, 0, new_cap - *used_cap);
    }
    *used = tmp;
    *used_cap = new_cap;
}

/*--- Helper: grow the terms/cubes arrays to at least 'needed' entries -------*/
static void ensure_terms_cap(size_t needed,
                             seq_t **terms,
                             seq_t **cubes,
                             size_t *terms_cap)
{
    if (needed <= *terms_cap)
        return;

    size_t new_cap = (*terms_cap) ? (*terms_cap * 2) : 16;
    if (needed > new_cap)
        new_cap = needed;

    seq_t *tmp_terms = (seq_t *)realloc(*terms, new_cap * sizeof(seq_t));
    seq_t *tmp_cubes  = (seq_t *)realloc(*cubes,  new_cap * sizeof(seq_t));
    if (!tmp_terms || !tmp_cubes)
        abort();

    *terms = tmp_terms;
    *cubes = tmp_cubes;
    *terms_cap = new_cap;
}

/*--- Main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    /* Static state retained across calls */
    static seq_t *terms = NULL;
    static seq_t *cubes = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;
    static seq_t next_candidate = 1;

    /* Initialize on first call */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        cubes = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        used_cap = 16;
        used = (uint8_t *)calloc(used_cap, 1);   /* zeroed */
        if (!terms || !cubes || !used)
            abort();
    }

    /* Generate sequence values up to index 'n' */
    while (terms_len < (size_t)n) {
        seq_t cand = next_candidate;

        /* Search for the next admissible candidate */
        for (;;) {
            size_t cand_idx = (size_t)cand;

            ensure_used_cap(cand_idx, &used, &used_cap);

            if (!used[cand_idx] && !is_sum_of_cubes(cand, cubes, terms_len))
                break;           /* admissible value found */

            ++cand;
        }

        /* Record the chosen value */
        size_t cand_idx = (size_t)cand;
        ensure_used_cap(cand_idx, &used, &used_cap);
        used[cand_idx] = 1;

        /* Grow terms/cubes arrays if necessary */
        ensure_terms_cap(terms_len + 1, &terms, &cubes, &terms_cap);

        terms[terms_len] = cand;
        cubes[terms_len] = cand * cand * cand;
        ++terms_len;

        next_candidate = cand + 1;    /* start searching from the next integer */
    }

    return terms[n - 1];
}
