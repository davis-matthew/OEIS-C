/*
 * A116836  Number of permutations of length n which avoid the patterns 2341,
 *          4132, 4321.
 * https://oeis.org/A116836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 7 a(n-1) - 19
 *            a(n-2) + 29 a(n-3) - 27 a(n-4) + 16 a(n-5) * - 10 a(n-6) + 7
 *            a(n-7) - 7 a(n-8) + 2 a(n-9) for n ≥ 10 * with initial values *
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=72, a(6)=233, * a(7)=739,
 *            a(8)=2343, a(9)=7458, a(10)=23801, …
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- common declarations ---------- */
static const seq_t init_vals[9] = {
    (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21, (seq_t)72,
    (seq_t)233, (seq_t)739, (seq_t)2343, (seq_t)7458
};

#ifdef __cplusplus
/* ---------- C++ implementation ---------- */
#include <vector>

static std::vector<seq_t> cache_vec;   /* cached terms, 0-based (a(1) → [0]) */

/* Compute term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but return 0 */
    if (n <= 9) return init_vals[n-1];

    /* initialize cache with initial terms if needed */
    if (cache_vec.empty())
        cache_vec.assign(init_vals, init_vals+9);

    /* extend cache up to n */
    for (idx_t i = cache_vec.size()+1; i <= n; ++i)
    {
        /* recurrence uses 0‑based indices: a(i) ↔ cache_vec[i-1]    */
        seq_t val =
            (seq_t)7  * cache_vec[i-2]   /* a(i-1)   */
          - (seq_t)19 * cache_vec[i-3]   /* a(i-2)   */
          + (seq_t)29 * cache_vec[i-4]   /* a(i-3)   */
          - (seq_t)27 * cache_vec[i-5]   /* a(i-4)   */
          + (seq_t)16 * cache_vec[i-6]   /* a(i-5)   */
          - (seq_t)10 * cache_vec[i-7]   /* a(i-6)   */
          + (seq_t)7  * cache_vec[i-8]   /* a(i-7)   */
          - (seq_t)7  * cache_vec[i-9]   /* a(i-8)   */
          + (seq_t)2  * cache_vec[i-10]; /* a(i-9)   */
        cache_vec.push_back(val);
    }
    return cache_vec[n-1];
}
#else
/* ---------- C implementation ---------- */
#include <stddef.h>
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache_c   = NULL;  /* dynamic array of cached terms */
static size_t cache_cap_c = 0;   /* capacity of cache_c */
static idx_t   cache_n_c   = 0;   /* number of terms currently stored */

/* Ensure the cache has room for at least n terms (1‑based). */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap_c >= (size_t)n) return;
    size_t new_cap = cache_cap_c ? cache_cap_c : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    /* realloc may return NULL on failure; this is not checked per
       requirement but would cause a runtime error if it fails.  */
    seq_t *tmp = (seq_t *)realloc(cache_c, new_cap * sizeof(seq_t));
    cache_c   = tmp;
    cache_cap_c = new_cap;
}

/* Compute term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but return 0 */
    if (n <= 9) return init_vals[n-1];

    /* allocate or extend cache */
    ensure_cache_size(n);

    /* extend cached terms up to n */
    for (idx_t i = cache_n_c; i < n; ++i)
    {
        if (i < 9)          /* initial terms */
            cache_c[i] = init_vals[i];
        else
        {
            /* recurrence uses 0‑based indices: a(i) ↔ cache_c[i-1]    */
            seq_t val =
                (seq_t)7  * cache_c[i-1]   /* a(i-1)   */
              - (seq_t)19 * cache_c[i-2]   /* a(i-2)   */
              + (seq_t)29 * cache_c[i-3]   /* a(i-3)   */
              - (seq_t)27 * cache_c[i-4]   /* a(i-4)   */
              + (seq_t)16 * cache_c[i-5]   /* a(i-5)   */
              - (seq_t)10 * cache_c[i-6]   /* a(i-6)   */
              + (seq_t)7  * cache_c[i-7]   /* a(i-7)   */
              - (seq_t)7  * cache_c[i-8]   /* a(i-8)   */
              + (seq_t)2  * cache_c[i-9];  /* a(i-9)   */
            cache_c[i] = val;
        }
    }
    cache_n_c = n;
    return cache_c[n-1];
}
#endif
