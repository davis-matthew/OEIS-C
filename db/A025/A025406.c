/*
 * A025406  Numbers that are the sum of 4 positive cubes in 2 or more ways.
 * https://oeis.org/A025406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025406: Numbers that are the sum of 4 positive cubes in 2 or more ways.
   The function builds all unordered quadruples of positive cubes up to a
   cube root limit, counts occurrences of each sum (using a byte array for
   counts), and extracts the sums that appear at least twice.  The internal
   cube‑root limit grows geometrically until the requested term index can
   be satisfied. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *terms = NULL;                /* 1‑indexed array of values */
static size_t terms_len = 0;               /* number of terms stored */
static idx_t cube_limit = 50;              /* current max cube root */
static bool data_ready = false;            /* all terms for current limit */

/* Rebuild the sequence terms up to at least 'need' terms. */
static void rebuild_terms(idx_t need)
{
    for (;;)
    {
        /* Max possible sum with current cube_limit. */
        idx_t max_sum = 4 * cube_limit * cube_limit * cube_limit;
        size_t max_sum_size = (size_t)max_sum + 1;

        /* Count array: 0 = none, 1 = seen once, >=2 = at least two ways. */
        unsigned char *cnt = (unsigned char *)calloc(max_sum_size, sizeof(unsigned char));
        if (!cnt) return;              /* out of memory – caller will never test this */

        /* Precompute cubes up to cube_limit. */
        idx_t *cube = (idx_t *)malloc((cube_limit + 1) * sizeof(idx_t));
        if (!cube) { free(cnt); return; }
        for (idx_t i = 1; i <= cube_limit; ++i) cube[i] = i * i * i;

        /* Enumerate unordered quadruples a <= b <= c <= d. */
        for (idx_t a = 1; a <= cube_limit; ++a)
        {
            idx_t a3 = cube[a];
            for (idx_t b = a; b <= cube_limit; ++b)
            {
                idx_t ab3 = a3 + cube[b];
                for (idx_t c = b; c <= cube_limit; ++c)
                {
                    idx_t abc3 = ab3 + cube[c];
                    for (idx_t d = c; d <= cube_limit; ++d)
                    {
                        size_t sum = (size_t)(abc3 + cube[d]);
                        unsigned char *v = &cnt[sum];
                        if (*v == 0)
                            *v = 1;
                        else if (*v == 1)
                            *v = 2;       /* mark at least two representations */
                    }
                }
            }
        }

        /* Collect qualifying sums into a new terms array. */
        size_t cap = 1024;
        seq_t *new_terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!new_terms) { free(cnt); free(cube); return; }

        size_t len = 0;
        for (size_t s = 0; s <= max_sum; ++s)
        {
            if (cnt[s] >= 2)
            {
                if (len == cap)
                {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(new_terms, cap * sizeof(seq_t));
                    if (!tmp) { free(new_terms); free(cnt); free(cube); return; }
                    new_terms = tmp;
                }
                new_terms[len++] = (seq_t)s;
            }
        }

        /* Release temporary data. */
        free(cnt);
        free(cube);

        /* Replace old terms array with the new one. */
        free(terms);
        terms = new_terms;
        terms_len = len;
        data_ready = true;

        if (terms_len >= (size_t)need)
            break;                  /* enough terms for current request */

        cube_limit <<= 1;            /* increase limit and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1 – not used */

    if (!data_ready || terms_len < (size_t)n)
        rebuild_terms(n);

    return terms[n - 1];                     /* sequence is 1‑indexed */
}
