/*
 * A054348  Triangular array whose rows are successive generations of the
 *          variant of the Kolakoski sequence described in A042942.
 * https://oeis.org/A054348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   expand_generation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054348: linearization of successive generations of the variant Kolakoski
   sequence (symbols 1 and 2, run‑lengths given by the sequence itself,
   starting with the symbol 2).  Generation n is obtained from generation n‑1 by
   expanding each run‑length; the sequence is read in rows, the triangle
   begins 2; 2,2; 2,2,1,1; ...  */
#include <stdlib.h>
#include "../utils/expand_generation.h"

static seq_t **gen_rows     = NULL;   /* pointers to each generation */
static size_t *gen_sizes   = NULL;    /* lengths of each generation */
static idx_t   cumulative  = 0;       /* total number of terms up to last gen */
static size_t  generations = 0;       /* number of generations stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (gen_rows == NULL) {
        gen_rows   = (seq_t **)malloc(sizeof(seq_t *));
        gen_sizes  = (size_t *)malloc(sizeof(size_t));
        if (!gen_rows || !gen_sizes) exit(1);

        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        if (!row0) exit(1);
        row0[0] = 2;
        gen_rows[0] = row0;
        gen_sizes[0] = 1;
        cumulative = 1;
        generations = 1;
    }

    /* extend the cache until we cover index n */
    while (cumulative <= n) {
        const seq_t *curr = gen_rows[generations - 1];
        size_t curr_len   = gen_sizes[generations - 1];

        seq_t *next = NULL;
        size_t next_len = 0;
        expand_generation(curr, curr_len, &next, &next_len);

        /* append new generation to the cache */
        gen_rows = (seq_t **)realloc(gen_rows,
                                     sizeof(seq_t *) * (generations + 1));
        gen_sizes = (size_t *)realloc(gen_sizes,
                                     sizeof(size_t) * (generations + 1));
        if (!gen_rows || !gen_sizes) exit(1);

        gen_rows[generations] = next;
        gen_sizes[generations] = next_len;
        cumulative += next_len;
        ++generations;
    }

    /* locate the generation that contains index n */
    idx_t prefix = cumulative - gen_sizes[generations - 1];
    size_t idx_in_gen = (size_t)(n - prefix);
    return gen_rows[generations - 1][idx_in_gen];
}
