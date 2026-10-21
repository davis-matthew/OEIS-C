/*
 * A093799  a(1) = 1; a(n+1) is the smallest triangular number such that the
 *          greatest common divisor gcd(a(n+1), a(n)) is a triangular number
 *          itself.
 * https://oeis.org/A093799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1, a(n) is the smallest triangle number T(k) such
 *            that gcd(T(k),a(n-1)) is also a triangle number. T(k) =
 *            k*(k+1)/2
 * Range:     u8 n <= 14, u16 n <= 192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_seq_t, _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic headers */
#include <stdint.h>
#include <stdlib.h>

/* Helper functions (provided by the evaluation environment) */
#include "../utils/_isqrt_seq.h"      /* seq_t _isqrt_seq(seq_t) */
#include "../utils/_gcd_seq_t.h"     /* seq_t _gcd_seq_t(seq_t,seq_t) */

/*------------------------------------------------------------*/
/*  In C++ we use a std::vector; in C we allocate raw arrays    */
/*------------------------------------------------------------*/
#if defined(__cplusplus)
# include <vector>
static std::vector<seq_t> a_cache;      /* a_cache[i] == a(i+1)   */
static std::vector<idx_t>  idx_cache;   /* idx_cache[i] == k for a(i+1)=T(k) */
#else
/* C: explicit dynamic arrays */
static seq_t*  a_cache  = NULL;
static idx_t*  idx_cache = NULL;
static size_t  cache_len = 0, cache_cap = 0;

static void ensure_cache (size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    a_cache   = (seq_t*)realloc(a_cache,  sizeof(seq_t)   * new_cap);
    idx_cache = (idx_t*)realloc(idx_cache, sizeof(idx_t)  * new_cap);
    cache_cap = new_cap;
}
#endif

/* Helper: check if a value is a triangular number */
static int is_triangular (seq_t x)
{
    if (x == 0) return 0;                      /* 0 is not triangular */
    seq_t tmp = 8 * x + 1;
    seq_t r   = _isqrt_seq(tmp);
    return (r * r == tmp);
}

/* Helper: compute the k-th triangular number */
static inline seq_t triangular (idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

/* Main sequence function */
seq_t a (idx_t n)
{
    if (n == 0) return 0;                      /* undefined, avoid UB */
    if (n == 1) return 1;                      /* a(1)=1 */

    size_t target = (size_t)n;                 /* 1‑based index */

#if defined(__cplusplus)
    /* Grow C++ vector as needed */
    while (a_cache.size() < target) {
        /* Ensure we have at least the first term */
        if (a_cache.empty()) {
            a_cache.push_back(1);            /* a(1)=1 */
            idx_cache.push_back(1);          /* T(1)=1 */
            continue;
        }

        /* Previous term information */
        const seq_t prev_val = a_cache.back();
        const idx_t  prev_k   = idx_cache.back();

        /* Start searching from the next triangle index */
        idx_t m = prev_k + 1;
        seq_t cand;
        while (1) {
            cand = triangular(m);
            seq_t g = _gcd_seq_t(cand, prev_val);
            if (is_triangular(g)) break;
            ++m;
        }

        a_cache.push_back(cand);
        idx_cache.push_back(m);
    }
    return a_cache[target - 1];
#else
    /* C: ensure arrays are large enough */
    ensure_cache(target);
    while (cache_len < target) {
        /* Initialize first term if cache empty */
        if (cache_len == 0) {
            a_cache[0]   = 1;                 /* a(1)=1 */
            idx_cache[0] = 1;                 /* T(1)=1 */
            ++cache_len;
            continue;
        }

        /* Previous term information */
        const seq_t prev_val = a_cache[cache_len - 1];
        const idx_t  prev_k   = idx_cache[cache_len - 1];

        /* Search for the next triangular number */
        idx_t m = prev_k + 1;
        seq_t cand;
        while (1) {
            cand = triangular(m);
            seq_t g = _gcd_seq_t(cand, prev_val);
            if (is_triangular(g)) break;
            ++m;
        }
        a_cache[cache_len]   = cand;
        idx_cache[cache_len] = m;
        ++cache_len;
    }
    return a_cache[target - 1];
#endif
}
