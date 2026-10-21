/*
 * A097633  Numbers not of form x^2 + y^2 + 7z^2.
 * https://oeis.org/A097633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers not of the form x^2 + y^2 + 7z^2.
   The function finds the n‑th integer which cannot be expressed
   as the sum of two ordinary squares and a square multiplied by 7
   by brute‑force enumeration of all triples (x, y, z). */

#include <stdlib.h>

/* --- static storage --- */
static idx_t          limit            = 0;      /* largest integer considered   */
static unsigned char  *repr            = NULL;   /* repr[i] = 1 if i is representable */
static size_t         repr_cap         = 0;      /* allocated size of repr array   */

static seq_t *nonrep                     = NULL;   /* array of non‑representable numbers */
static size_t  nonrep_cnt                = 0;      /* number of stored terms          */
static size_t  nonrep_cap                = 0;      /* capacity of nonrep array        */

/* --- helpers --- */
static void ensure_repr_capacity(idx_t new_limit)
{
    size_t new_cap = (size_t)new_limit + 1;           /* need indices 0 .. new_limit */
    if (repr_cap < new_cap) {
        unsigned char *tmp = (unsigned char *)realloc(repr, new_cap);
        if (!tmp) abort();
        repr = tmp;
        repr_cap = new_cap;
    }
}

static void ensure_nonrep_capacity(idx_t up_to)
{
    size_t new_cap = (size_t)up_to + 1;
    if (nonrep_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(nonrep, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        nonrep = tmp;
        nonrep_cap = new_cap;
    }
}

static void recompute_representation(idx_t up_to)
{
    /* clear previous information */
    for (idx_t i = 0; i <= up_to; ++i) repr[i] = 0;

    /* enumerate all triples (x, y, z) */
    for (idx_t z = 0; ; ++z) {
        seq_t sq_z = (seq_t)7 * z * z;
        if (sq_z > (seq_t)up_to) break;
        for (idx_t x = 0; ; ++x) {
            seq_t sq_x = (seq_t)x * x;
            seq_t sum_zx = sq_z + sq_x;
            if (sum_zx > (seq_t)up_to) break;
            for (idx_t y = 0; ; ++y) {
                seq_t sum = sum_zx + (seq_t)y * y;
                if (sum > (seq_t)up_to) break;
                repr[(idx_t)sum] = 1;
            }
        }
    }

    /* collect numbers that are not representable */
    ensure_nonrep_capacity(up_to);
    nonrep_cnt = 0;
    for (idx_t i = 1; i <= up_to; ++i) {
        if (!repr[i]) {
            nonrep[nonrep_cnt++] = (seq_t)i;
        }
    }
}

/* --- public function --- */
seq_t a(idx_t n)
{
    /* n is 1‑based according to the problem statement */
    if (n == 0) abort();   /* undefined for offset 0 */

    /* increase the search limit until we have at least n terms */
    while (nonrep_cnt < n) {
        idx_t new_limit = (limit == 0) ? n * 16 : limit * 2;
        if (new_limit < n * 16) new_limit = n * 16;
        limit = new_limit;

        ensure_repr_capacity(limit);
        recompute_representation(limit);
    }

    return nonrep[n - 1];
}
