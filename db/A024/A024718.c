/*
 * A024718  a(n) = (1/2)*(1 + Sum_{k=0..n} binomial(2*k, k)).
 * https://oeis.org/A024718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + sum_{k=0..n} binomial(2*k, k)) / 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by maintaining the partial sum of the
 * central binomial coefficients using the recurrence
 *
 *    C(2k, k) = (C(2k-2, k-1) / k) * 2*(2k-1)    (k ≥ 1)
 *
 * and then adding 1 and dividing by 2 to obtain a(n).  A static
 * cache of already computed terms is used so that calls are fast
 * even if the arguments are not monotonic.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ===== C implementation ===== */
#ifndef __cplusplus

static seq_t *c_cache      = NULL;   /* array of cached values   */
static size_t c_cache_cap  = 0;      /* allocated capacity      */
static size_t c_cache_size = 0;      /* next index to compute  */
static seq_t c_sum_central = 0;      /* ∑_{k=0}^{i-1} C(2k,k)    */
static seq_t c_last_binom  = 0;      /* C(2(i-1), i-1)           */

static void ensure_c_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* Initial allocation */
    if (c_cache == NULL) {
        c_cache_cap = needed > 2 ? needed : 2;
        c_cache = (seq_t *)malloc(c_cache_cap * sizeof(seq_t));
        if (!c_cache) exit(EXIT_FAILURE);
        c_cache_size = 0;
        c_sum_central = 0;
        c_last_binom  = 0;
    }

    /* Grow if necessary */
    if (needed > c_cache_cap) {
        size_t new_cap = c_cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        c_cache = tmp;
        c_cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = c_cache_size; i <= n; ++i) {
        if (i == 0) {
            c_cache[0] = (seq_t)1;
            c_sum_central = (seq_t)1;
            c_last_binom  = (seq_t)1;
        } else {
            seq_t new_binom = (c_last_binom / (seq_t)i) *
                              (seq_t)(2 * (2 * i - 1));
            c_last_binom  = new_binom;
            c_sum_central = c_sum_central + new_binom;
            c_cache[i] = (c_sum_central + (seq_t)1) / (seq_t)2;
        }
        ++c_cache_size;
    }
}

#endif /* __cplusplus */

/* ===== C++ implementation ===== */
#ifdef __cplusplus

static std::vector<seq_t> cpp_cache;   /* static cache vector   */
static idx_t cpp_next_index = 0;      /* next index to compute*/
static seq_t cpp_sum_central = 0;     /* ∑_{k=0}^{i-1} C(2k,k)   */
static seq_t cpp_last_binom  = 0;     /* C(2(i-1), i-1)          */

static void ensure_cpp_cache(idx_t n)
{
    if ((size_t)n + 1 > cpp_cache.size())
        cpp_cache.resize((size_t)n + 1);

    for (idx_t i = cpp_next_index; i <= n; ++i) {
        if (i == 0) {
            cpp_cache[0] = (seq_t)1;
            cpp_sum_central = (seq_t)1;
            cpp_last_binom  = (seq_t)1;
        } else {
            seq_t new_binom = (cpp_last_binom / (seq_t)i) *
                              (seq_t)(2 * (2 * i - 1));
            cpp_last_binom = new_binom;
            cpp_sum_central = cpp_sum_central + new_binom;
            cpp_cache[i] = (cpp_sum_central + (seq_t)1) / (seq_t)2;
        }
        ++cpp_next_index;
    }
}

#endif /* __cplusplus */

/* ===== Public interface ===== */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    ensure_cpp_cache(n);
    return cpp_cache[(size_t)n];
#else
    ensure_c_cache(n);
    return c_cache[(size_t)n];
#endif
}
