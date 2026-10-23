/*
 * A170089  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 * Σ_{k=1..36} a(n-k) + 465 * a(n-37) + c(n) where
 *            c(0)=1, c(n)=2 for 1≤n≤36, c(37)=1, c(n)=0 otherwise. Offset 0:
 *            a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile as C11 or C++17.  */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* static cache for memoisation */
static seq_t *a_cache_c = NULL;   /* used in C build */
static size_t a_cap_c   = 0;
static size_t a_len_c   = 0;

#ifdef __cplusplus
static std::vector<seq_t> a_cache_cpp;   /* used in C++ build */
#endif

static inline seq_t c_value(idx_t n) {
    if (n == 0)                 return (seq_t)1;
    else if (n <= 36)           return (seq_t)2;
    else if (n == 37)           return (seq_t)1;
    else                        return (seq_t)0;
}

seq_t a(idx_t n)
{
    const size_t needed = (size_t)n + 1;

#ifdef __cplusplus
    if (a_cache_cpp.size() < needed) {
        a_cache_cpp.reserve(needed);
        size_t start = a_cache_cpp.size();

        for (size_t i = start; i < needed; ++i) {
            seq_t sumPrev = 0;
            for (size_t k = 1; k <= 36 && i >= k; ++k)
                sumPrev += a_cache_cpp[i - k];

            seq_t val = sumPrev * (seq_t)30;
            if (i >= 37)
                val += (seq_t)465 * a_cache_cpp[i - 37];

            val += c_value((idx_t)i);
            a_cache_cpp.push_back(val);
        }
    }
    return a_cache_cpp[(size_t)n];
#else
    /* Grow cache if necessary (C build) */
    if (needed > a_cap_c) {
        size_t new_cap = a_cap_c > 0 ? a_cap_c : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache_c, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failed */
        a_cache_c = tmp;
        a_cap_c   = new_cap;
    }

    /* Compute values up to 'n' */
    if (a_len_c < needed) {
        for (size_t i = a_len_c; i < needed; ++i) {
            seq_t sumPrev = 0;
            for (size_t k = 1; k <= 36 && i >= k; ++k)
                sumPrev += a_cache_c[i - k];

            seq_t val = sumPrev * (seq_t)30;
            if (i >= 37)
                val += (seq_t)465 * a_cache_c[i - 37];

            val += c_value((idx_t)i);
            a_cache_c[i] = val;
        }
        a_len_c = needed;
    }
    return a_cache_c[(size_t)n];
#endif
}
