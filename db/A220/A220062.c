/*
 * A220062  Number A(n,k) of n length words over k-ary alphabet, where
 *          neighboring letters are neighbors in the alphabet; square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A220062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 292, u32 n <= 934, u64 n <= 2990,
 *            u128 n <= 9620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220062:
   Number A(n,k) = number of n‑length words over a k‑ary alphabet
   where consecutive letters are neighbors in the alphabet.
   Equivalent to the number of walks of length n‑1 on the path
   graph P_k.  The sequence a(N) is read by antidiagonals:
   a(0) = 1,  a(1) = 1,  a(2) = 0,  a(3) = 1,  a(4) = 1, ...
   This implementation computes a(N) by locating the
   pair (i,k) on the n‑k antidiagonal and then counting walks
   with dynamic programming.  It works for all supported
   seq_t types (unsigned integer or arbitrary‑precision) and
   is safe in both C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#  include <new>
#endif

/* locate (i,k) from index N on the antidiagonal */
static void antidiagonal_coords(idx_t N, idx_t *i, idx_t *k)
{
    /* find largest d with d*(d+1)/2 <= N */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= N)
        ++d;
    idx_t offset = N - d * (d + 1) / 2;
    *i = offset;
    *k = d - offset;
}

/* allocate an array of seq_t of length k, with correct construction */
static seq_t *alloc_seq_array(idx_t k)
{
#ifdef __cplusplus
    return new seq_t[(size_t)k];
#else
    return (seq_t *)malloc((size_t)k * sizeof(seq_t));
#endif
}

/* deallocate the array */
static void free_seq_array(seq_t *p, idx_t k)
{
    if (!p) return;
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* number of walks of length i-1 on path P_k */
static seq_t walk_count(idx_t i, idx_t k)
{
    if (k == 0)              /* empty alphabet */
        return (i == 0) ? 1 : 0;
    if (i == 0)              /* empty word */
        return 1;

    /* allocate two rows */
    seq_t *cur  = alloc_seq_array(k);
    seq_t *next = alloc_seq_array(k);
    if (!cur || !next) {
        free_seq_array(cur, k);
        free_seq_array(next, k);
        return 0;            /* allocation failed */
    }

    /* walk of length 0: one way to start at each vertex */
    for (idx_t v = 0; v < k; ++v)
        cur[v] = 1;

    /* propagate for steps 1 .. i-1 */
    for (idx_t step = 1; step < i; ++step) {
        for (idx_t v = 0; v < k; ++v) {
            seq_t val = 0;
            if (v > 0)       val += cur[v - 1];
            if (v + 1 < k)   val += cur[v + 1];
            next[v] = val;
        }
        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum walks ending at any vertex */
    seq_t total = 0;
    for (idx_t v = 0; v < k; ++v)
        total += cur[v];

    free_seq_array(cur, k);
    free_seq_array(next, k);
    return total;
}

/* required entry point */
seq_t a(idx_t N)
{
    idx_t i, k;
    antidiagonal_coords(N, &i, &k);
    return walk_count(i, k);
}
