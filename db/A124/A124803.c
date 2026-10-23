/*
 * A124803  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A124803 – Number of base‑31 circular n‑digit numbers with adjacent
    digits differing by at most 1.
    a(n) = trace(A^n),  where A is the 31×31 matrix with 1 on the
    diagonal and on the two off‑diagonals (i.e. neighbours or the
    same digit).  The matrix A models the allowed transitions, so
    trace(A^n) counts all closed walks of length n, i.e. all admissible
    circular words of length n.  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

#define BASE 31UL                     /* alphabet size */
#define DIM  ((size_t)BASE * BASE)    /* matrix dimension */

/* static storage for matrices and memoised values */
static seq_t *cur  = NULL;      /* M^k  (current power)   */
static seq_t *next = NULL;      /* temporary matrix      */
static seq_t *cache = NULL;     /* cache of a(k)         */
static size_t cache_cap = 0;    /* current cache capacity */
static idx_t  max_computed = 0; /* largest n whose value is cached */

/* initialise matrices and cache once */
static void init_once(void)
{
    static int initialized = 0;
    if (initialized)
        return;
    initialized = 1;

    /* allocate matrices */
    cur  = alloc_seq_array(DIM);
    next = alloc_seq_array(DIM);
    if (!cur || !next)
        abort();

    /* zero matrices */
    for (size_t i = 0; i < DIM; ++i) {
        cur[i]  = 0;
        next[i] = 0;
    }

    /* identity matrix (M^0) */
    for (int i = 0; i < BASE; ++i)
        cur[i * BASE + i] = 1;

    /* initial cache value a(0) = 1 */
    cache = alloc_seq_array(1);
    if (!cache) abort();
    cache[0] = 1;
    cache_cap = 1;
    max_computed = 0;
}

/* grow cache array to hold values up to at least n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap <= (size_t)(n + 1))
        new_cap = (size_t)(n + 1);
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();
    /* copy old values */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(k) for all k ≤ n, storing them in cache */
static void extend_to(idx_t n)
{
    init_once();
    if (max_computed >= n)
        return;

    for (idx_t step = max_computed + 1; step <= n; ++step) {
        /* zero next matrix */
        for (size_t i = 0; i < DIM; ++i)
            next[i] = 0;

        /* next = cur * M  (M has 1 on diagonal and neighbours) */
        for (int i = 0; i < BASE; ++i) {
            size_t row = (size_t)i * BASE;
            for (int k = 0; k < BASE; ++k) {
                seq_t val = cur[row + k];
                if (k > 0)
                    next[row + k - 1] += val;
                next[row + k] += val;
                if (k + 1 < BASE)
                    next[row + k + 1] += val;
            }
        }

        /* trace of next = trace(M^step) */
        seq_t trace = 0;
        for (int i = 0; i < BASE; ++i)
            trace += next[(size_t)i * BASE + i];

        /* store result */
        ensure_cache_capacity(step);
        cache[step] = trace;

        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }
    max_computed = n;
}

/* ================================================================
   public API
   ================================================================ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */
    extend_to(n);
    return cache[n];
}
