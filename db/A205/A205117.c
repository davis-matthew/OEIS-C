/*
 * A205117  The number s(j) such that n divides s(k)-s(j), where s(j) is the
 *          j-th Lucas number and k is the least positive integer for which
 *          such a j with 0<j<k exists.
 * https://oeis.org/A205117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 150, u32 n <= 228, u64 n <= 514,
 *            u128 n <= 954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each n>0 find the first Lucas number L_j such that
   there is a later index k>j with n | (L_k – L_j).  This is equivalent
   to the earliest index j whose Lucas residue modulo n repeats at some
   later index.  The function returns L_j.  No lookup table is used; the
   result is recomputed (and memoised) on demand. */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[n] holds a(n), 1‑based; cache[0] unused */
static idx_t  cache_size = 0;        /* number of valid cache entries */

/* Grow cache array to hold values up to index n */
static void grow_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) abort();
        memset(cache, 0, (size_t)cache_size * sizeof(seq_t));
        return;
    }
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_cache) abort();
        memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
        memset(new_cache + cache_size, 0, 
               (size_t)(new_size - cache_size) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* unused, keep safe */

    grow_cache(n);

    if (cache[n] != 0) return cache[n];

    /* allocate array to record the first occurrence of each residue */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) abort();
    memset(first, 0, (size_t)n * sizeof(idx_t));

    idx_t r1 = 1 % n;          /* residue of L1 (1) */
    idx_t r2 = 3 % n;          /* residue of L2 (3) */

    first[r1] = 1;
    if (r2 == r1) {
        cache[n] = (seq_t)1;   /* j = 1 */
        free(first);
        return cache[n];
    }
    first[r2] = 2;

    /* iterate through Lucas residues until a repeat is found */
    idx_t prev_r = r1, curr_r = r2;
    seq_t prev_L = (seq_t)1, curr_L = (seq_t)3;
    idx_t i = 3;               /* next index to compute */
    idx_t j = 0;               /* index of the repeated residue */

    while (1) {
        seq_t L    = prev_L + curr_L;          /* next Lucas number (unmodded) */
        idx_t r    = (prev_r + curr_r) % n;    /* next residue modulo n */

        if (first[r] == 0)
            first[r] = i;
        else {
            j = first[r];
            break;
        }

        prev_r = curr_r;      curr_r = r;
        prev_L = curr_L;      curr_L = L;
        ++i;
    }

    /* compute L_j (may require recomputation) */
    seq_t result;
    if (j == 1) {
        result = (seq_t)1;
    } else if (j == 2) {
        result = (seq_t)3;
    } else {
        seq_t p = (seq_t)1;   /* L1 */
        seq_t q = (seq_t)3;   /* L2 */
        idx_t ii;
        for (ii = 3; ii <= j; ++ii) {
            seq_t tmp = p + q;
            p = q;
            q = tmp;
        }
        result = q; /* q now holds L_j */
    }

    cache[n] = result;
    free(first);
    return result;
}
