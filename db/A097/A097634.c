/*
 * A097634  Numbers not of form x^2 + 2y^2 + 2yz + 4z^2.
 * https://oeis.org/A097634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097634 – Numbers not of the form x^2 + 2y^2 + 2yz + 4z^2
 *
 * The function below builds a table of all non‑representable numbers up
 * to a limit that is increased until it contains at least n terms.
 * The representation check is done by brute force enumeration of all
 * (z, y, x) values that fit into the current limit.
 *
 * Complexity is roughly O(L^1.5) for a limit L; for the 10 000th term
 * the limit stays below a few hundred thousand, so the routine works
 * comfortably within the time limits.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static seq_t *cached_terms = NULL;   /* array of non‑representable numbers */
static size_t  cached_len   = 0;      /* number of elements in the array */
static size_t  cur_limit    = 0;      /* current upper bound on values processed */

/* Build the array of non‑representable numbers up to 'limit'.
 * The function returns the number of terms in the array via 'out_len'.
 * The caller is responsible for freeing the returned array. */
static seq_t *build_terms(size_t limit, size_t *out_len)
{
    bool *rep = (bool *)calloc(limit + 1, sizeof(bool));
    if (rep == NULL) return NULL;

    /* Determine bounds for z and y.  They are small enough that a
       double precision sqrt is safe for the calculations below. */
    int zmax = (int)(sqrt((double)limit / 4.0) + 2.0);
    int ymax_base = (int)(sqrt((double)limit / 2.0) + 2.0);

    for (int z = -zmax; z <= zmax; ++z) {
        int64_t zsq   = (int64_t)z * z;
        int64_t base_z = 4 * zsq;
        if (base_z > (int64_t)limit) continue;

        for (int y = -ymax_base; y <= ymax_base; ++y) {
            int64_t ysq   = (int64_t)y * y;
            int64_t base_y = 2 * ysq + 2LL * y * z;
            int64_t total_without_x = base_z + base_y;
            if (total_without_x > (int64_t)limit) continue;
            if (total_without_x < 0) continue;          /* overflow guard */

            int64_t rem = (int64_t)limit - total_without_x;
            int xmax = (int)(sqrt((double)rem) + 1.0);
            for (int x = 0; x <= xmax; ++x) {
                int64_t val = total_without_x + (int64_t)x * x;
                if (val <= (int64_t)limit) rep[(size_t)val] = true;
            }
        }
    }

    /* Collect the non‑representable numbers. */
    size_t est = limit;                     /* at most 'limit' numbers */
    seq_t *terms = (seq_t *)malloc(est * sizeof(seq_t));
    if (terms == NULL) {
        free(rep);
        return NULL;
    }

    size_t cnt = 0;
    for (size_t i = 1; i <= limit; ++i) {
        if (!rep[i]) {
            terms[cnt++] = (seq_t)i;
        }
    }

    free(rep);
    terms = (seq_t *)realloc(terms, cnt * sizeof(seq_t));
    *out_len = cnt;
    return terms;
}

/* Ensure that cached_terms has at least 'n' elements.
 * If not, grow the limit, rebuild the array, and free the old one. */
static void ensure_terms(idx_t n)
{
    if (cached_terms != NULL && cached_len >= (size_t)n) return;

    /* Initialise or grow the limit. */
    if (cur_limit == 0) cur_limit = 1024;     /* start small */
    else cur_limit *= 2;                      /* double each time we need more */

    /* Build the new table. */
    seq_t *new_terms = build_terms(cur_limit, &cached_len);
    if (new_terms == NULL) return;           /* allocation failure – unlikely */

    /* Free old table and replace. */
    free(cached_terms);
    cached_terms = new_terms;

    /* In the unlikely event that the new table is still too small,
       repeat the growth process until it contains 'n' terms. */
    while (cached_len < (size_t)n) {
        cur_limit *= 2;
        new_terms = build_terms(cur_limit, &cached_len);
        if (new_terms == NULL) break;        /* allocation failure – stop */
        free(cached_terms);
        cached_terms = new_terms;
    }
}

/* ------------------------------------------------------------- */
/* Public function as required by the test harness.             */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_terms(n);
    if ((size_t)n <= cached_len)
        return cached_terms[n - 1];
    else
        return (seq_t)0;   /* Should never happen for valid inputs */
}
