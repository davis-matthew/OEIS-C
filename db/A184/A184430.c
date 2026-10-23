/*
 * A184430  Upper s-Wythoff sequence, where s=A002620 (quarter squares).
 *          Complement of A184429.
 * https://oeis.org/A184430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*=====  A184430 Upper s‑Wythoff sequence: a(n)  =====*/
/* This implementation generates the sequence by producing the
   pairs (L(k), U(k)) for k = 1, 2, …, where
   s_k = ⌊k²/4⌋,
   L(k) = the smallest positive integer not yet used,
   U(k) = L(k) + s_k + ⌈k/2⌉.
   The numbers L(k) form the complementary lower sequence, and
   U(k) are the terms of this sequence.  The algorithm keeps a
   sorted list of all numbers seen so far so that the next lower
   term can be found in O(log n).  All values are stored in
   the static cache seq_cache[1 … max_cached] for repeated
   queries.  Complexity: O(n) per call of a(n) for n up to the
   requested value.  All arithmetic is performed in seq_t to
   guarantee exactness for values that fit in the target type.   */

static idx_t    max_cached = 0;          /* largest n cached so far */
static size_t   cache_cap  = 0;          /* capacity of seq_cache   */
static seq_t   *seq_cache  = NULL;       /* seq_cache[n] = a(n)     */

static size_t   used_cap      = 0;       /* capacity of used_arr    */
static size_t   used_len      = 0;       /* number of used numbers  */
static seq_t  *used_arr      = NULL;     /* sorted list of used nums*/
static seq_t    lower_candidate = 1;     /* next candidate for L(k) */

/*-----  Binary search: does used_arr contain v?  -----*/
static int used_contains (seq_t v)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < used_len) && (used_arr[lo] == v);
}

/*-----  Insert v keeping used_arr sorted  -----*/
static void used_insert (seq_t v)
{
    /* grow capacity if needed */
    if (used_len == used_cap)
    {
        used_cap = used_cap ? used_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc (used_arr, used_cap * sizeof (seq_t));
        if (!tmp) exit (EXIT_FAILURE);
        used_arr = tmp;
    }
    /* shift larger elements to the right */
    size_t i;
    for (i = used_len; i > 0 && used_arr[i-1] > v; --i)
        used_arr[i] = used_arr[i-1];
    used_arr[i] = v;
    ++used_len;
}

/*-----  Build cache up to at least n  -----*/
static void ensure_cache (idx_t n)
{
    while (max_cached < n)
    {
        idx_t k = max_cached + 1;        /* next pair index */

        /* --- find lower term L(k) --- */
        seq_t lower = lower_candidate;
        while (used_contains (lower)) ++lower;

        /* mark lower as used */
        used_insert (lower);

        /* --- compute upper term U(k) --- */
        seq_t sk        = ((seq_t)k * (seq_t)k) / 4;          /* ⌊k²/4⌋   */
        seq_t ceil_half = ((seq_t)k + 1) >> 1;                /* ⌈k/2⌉    */
        seq_t upper     = lower + sk + ceil_half;

        /* mark upper as used */
        used_insert (upper);

        /* --- store result --- */
        if (k >= cache_cap)      /* grow cache if necessary */
        {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc (seq_cache, new_cap * sizeof (seq_t));
            if (!tmp) exit (EXIT_FAILURE);
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[k] = upper;
        max_cached = k;

        /* next search starts after the last lower chosen */
        lower_candidate = lower + 1;
    }
}

/*-----  Public function required by the build system  -----*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n == 0 */

    /* initialise caches on first call */
    if (!cache_cap)
    {
        cache_cap = 8;
        seq_cache = (seq_t *)calloc (cache_cap, sizeof (seq_t));
        if (!seq_cache) exit (EXIT_FAILURE);
    }
    if (!used_cap)
    {
        used_cap = 8;
        used_arr = (seq_t *)calloc (used_cap, sizeof (seq_t));
        if (!used_arr) exit (EXIT_FAILURE);
    }

    ensure_cache (n);
    return seq_cache[n];
}
