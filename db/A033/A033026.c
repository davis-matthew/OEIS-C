/*
 * A033026  Numbers whose base-13 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 230, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033026 – numbers whose base‑13 expansion has no run of digits with length < 2
   (every run of equal digits has length ≥ 2).  The function generates all
   admissible base‑13 strings in increasing numeric order, caches them,
   and returns the n‑th term (a(1) is the first listed term). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define PARENT_SINK 13   /* sentinel value meaning “no previous digit” */

/* ----- Static cache for generated terms ----- */
static seq_t *cache   = NULL; /* sorted array of numbers found so far */
static size_t cache_len = 0;  /* how many numbers we have */
static size_t cache_cap = 0;  /* current allocated capacity */
static int last_len   = 1;    /* length of the last processed number */

/* ----- Recursive generation of admissible base‑13 strings ----- */
static void gen_rec(int pos, int L, int prev, int flag, seq_t cur)
{
    if (pos == L) {
        if (flag == 0) {                     /* only complete runs are accepted */
            if (cache_len == cache_cap) {    /* grow cache if needed */
                cache_cap = cache_cap ? cache_cap * 2 : 1024;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur;
        }
        return;
    }

    if (flag == 1) {
        /* we have placed the first digit of a run – must repeat it */
        gen_rec(pos + 1, L, prev, 0, cur * (seq_t)13 + (seq_t)prev);
    } else {
        for (int d = 0; d <= 12; ++d) {
            if (prev == PARENT_SINK && d == 0) /* leading zero not allowed */
                continue;

            if (prev != PARENT_SINK && d == prev) {
                /* continue the current run (length ≥ 2 now) */
                gen_rec(pos + 1, L, d, 0, cur * (seq_t)13 + (seq_t)d);
            } else {
                /* start a new run – first digit of it */
                gen_rec(pos + 1, L, d, 1, cur * (seq_t)13 + (seq_t)d);
            }
        }
    }
}

/* Generate all valid numbers of a given length L */
static void generate_length(int L)
{
    gen_rec(0, L, PARENT_SINK, 0, (seq_t)0);
}

/* ----- Main function required by the build ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, return 0 for safety */

    if (cache_len >= n)               /* cache already contains the term */
        return cache[n - 1];

    /* generate additional lengths until we have at least n terms */
    while (cache_len < n) {
        int L = last_len + 1;
        if (L < 2) L = 2;              /* skip length‑1 numbers (none exist) */
        generate_length(L);
        last_len = L;
    }

    return cache[n - 1];
}
