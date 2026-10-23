/*
 * A034035  Not of form (x^2+xy+2y^2 with x >= 0, y >= 0).
 * https://oeis.org/A034035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034035: integers that are NOT of the form
   x^2 + x*y + 2*y^2 with x >= 0, y >= 0.
   The algorithm keeps a cache of the first n non‑representable
   numbers and enlarges it as needed. */
#include <stdlib.h>
#include <stdbool.h>

/* static state for caching non‑representable numbers */
static seq_t *non_repr = NULL;   /* array of found numbers */
static idx_t  non_len   = 0;      /* number of elements stored   */
static idx_t  non_cap   = 0;      /* capacity of non_repr array   */
static idx_t  proc_lim  = 0;      /* largest number considered so far */

/* enlarge the cache until it contains at least the first 'n' non‑representables */
static void ensure_cache(idx_t n)
{
    while (non_len < n) {
        /* choose new upper limit */
        idx_t new_lim = (proc_lim == 0) ? 100 : proc_lim * 2;

        /* allocate a bitmap of representable numbers up to new_lim */
        size_t arrsz = (size_t)new_lim + 1;          /* include 0 index */
        char *rep = (char *)calloc(arrsz, 1);
        if (!rep) exit(1); /* fatal: out of memory */

        /* enumerate all numbers of the form x^2 + x*y + 2*y^2 <= new_lim */
        for (idx_t y = 0; ; ++y) {
            seq_t base = (seq_t)2 * (seq_t)y * (seq_t)y;   /* 2*y^2 */
            if (base > new_lim) break;                   /* no more y */

            for (seq_t x = 0; ; ++x) {
                seq_t val = (seq_t)x * (seq_t)x + (seq_t)x * (seq_t)y + base;
                if (val > new_lim) break;                /* x too large */
                rep[(size_t)val] = 1;                    /* mark representable */
            }
        }

        /* collect the non‑representable numbers in ascending order */
        for (idx_t i = proc_lim + 1; i <= new_lim; ++i) {
            if (!rep[(size_t)i]) {                      /* i is not representable */
                if (non_len == non_cap) {               /* grow the array */
                    idx_t new_cap = (non_cap == 0) ? 128 : non_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(non_repr,
                                                (size_t)new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    non_repr = tmp;
                    non_cap = new_cap;
                }
                non_repr[non_len++] = (seq_t)i;
            }
        }

        proc_lim = new_lim;          /* update the considered limit */
        free(rep);                   /* free the temporary bitmap */
    }
}

/* public entry point: return the n-th (1‑based) non‑representable number */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behavior below offset is undefined */
    ensure_cache(n);
    return non_repr[n - 1];
}
