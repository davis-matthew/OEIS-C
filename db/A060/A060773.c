/*
 * A060773  Numbers having a unique partition into three nonnegative
 *          triangular numbers.
 * https://oeis.org/A060773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th nonnegative integer that has a unique partition into
 *            three nonnegative triangular numbers. Triangular number T(i) =
 *            i*(i+1)/2
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Helper types – provided by the build system */
extern void *xmalloc(size_t);    /* just a wrapper for malloc */

/* -------------------------------------------------------------------------- */
/*  Dynamic arrays for triangular numbers, a table of triangularness,
    and the sequence itself.  All static so that memoisation is shared
    across calls. */
static idx_t   *tri = NULL;          /* triangular numbers  */
static size_t   tri_len   = 0;        /* current number of entries   */
static size_t   tri_cap   = 0;        /* allocation capacity         */
static idx_t   next_tri_idx = 0;      /* next triangular index to
                                       (tri[ next_tri_idx ] is next triangle to compute) */

static char    *is_tri = NULL;       /* is_tri[x] == 1 iff x is triangular */
static idx_t    is_tri_size = 0;     /* current size of is_tri array   */

static seq_t   *seq = NULL;          /* gathered sequence values          */
static size_t   seq_len   = 0;        /* length of seq                   */
static size_t   seq_cap   = 0;        /* capacity of seq                 */

static idx_t   next_candidate = 0;    /* next integer to test for unique
                                       partition property */

/* -------------------------------------------------------------------------- */
/*  Grow capacity of triangular table */
static void ensure_tri_data(idx_t up_to)
{
    if (up_to < is_tri_size)
        return;                       /* already have all we need */

    size_t new_size = up_to + 1;
    char *tmp = (char *)realloc(is_tri, new_size);
    if (!tmp)
        return;                       /* allocation failed – assume never */
    memset(tmp + is_tri_size, 0, new_size - is_tri_size);
    is_tri = tmp;
    is_tri_size = new_size;

    /* Compute triangular numbers that are ≤ up_to and fill tri[], is_tri[] */
    for (idx_t i = next_tri_idx; ; ++i) {
        idx_t t = i * (i + 1) / 2;
        if (t > up_to)
            break;
        if (tri_len == tri_cap) {
            tri_cap = tri_cap ? tri_cap * 2 : 8;
            idx_t *tmp2 = (idx_t *)realloc(tri, tri_cap * sizeof(idx_t));
            if (!tmp2)
                return;               /* allocation failed – assume never */
            tri = tmp2;
        }
        tri[tri_len++] = t;
        if (t < new_size)             /* should always be true */
            is_tri[t] = 1;
        next_tri_idx = i + 1;
    }
}

/*  Grow the sequence array to at least min_cap elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        return;                       /* allocation failed – assume never */
    seq = tmp;
    seq_cap = new_cap;
}

/*  Test whether n has exactly one unordered triple of triangular numbers
    whose sum equals n.  Returns true if unique. */
static bool has_unique_tri_partition(idx_t n)
{
    int count = 0;

    for (size_t ii = 0; ii < tri_len && tri[ii] <= n; ++ii) {
        idx_t ti = tri[ii];
        for (size_t jj = ii; jj < tri_len; ++jj) {
            idx_t tj = tri[jj];
            idx_t sum_ti_tj = ti + tj;
            if (sum_ti_tj > n)
                break;
            idx_t rem = n - sum_ti_tj;
            if (rem < tj)                /* enforce ti <= tj <= tk */
                continue;
            if (rem < is_tri_size && is_tri[rem]) {
                ++count;
                if (count > 1)            /* not unique – short‑circuit */
                    return false;
            }
        }
    }
    return (count == 1);
}

/* -------------------------------------------------------------------------- */
/*  Public entry point.  For a given index n (0‑based) return a(n). */
seq_t a(idx_t n)
{
    /* Make sure we have computed enough terms. */
    for (;; ++next_candidate) {
        if (seq_len > n)
            return seq[n];

        /* The next integer we test might be larger than the current
           is_tri array size – grow it. */
        ensure_tri_data(next_candidate);

        if (has_unique_tri_partition(next_candidate)) {
            /* Add to sequence */
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)next_candidate;
        }
    }
}
