/*
 * A171373  Binomial transform of A171372.
 * https://oeis.org/A171373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=16, a(3)=36, a(4)=76;
 *            a(n)=5*a(n-1)-10*a(n-2)+10*a(n-3)-5*a(n-4)+2*a(n-5) for n≥5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* ---------- C implementation ---------- */
#ifndef __cplusplus
static seq_t *a_cache_c = NULL;    /* dynamic array of seq_t for C build */
static size_t  a_cap_c   = 0;      /* allocated capacity */
static size_t  a_size_c  = 0;      /* number of elements actually computed */

static void init_cache_c(void)
{
    if (a_cache_c == NULL) {
        a_cap_c = 16;
        a_cache_c = (seq_t *)realloc(a_cache_c, a_cap_c * sizeof(seq_t));
        if (!a_cache_c) abort();
        a_cache_c[0] = (seq_t)1;
        a_cache_c[1] = (seq_t)6;
        a_cache_c[2] = (seq_t)16;
        a_cache_c[3] = (seq_t)36;
        a_cache_c[4] = (seq_t)76;
        a_size_c = 5;
    }
}

static void ensure_cache_c(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (a_cache_c == NULL) init_cache_c();
    if (needed <= a_cap_c) return;
    size_t new_cap = a_cap_c ? a_cap_c * 2 : 16;
    if (new_cap < needed) new_cap = needed * 2;
    seq_t *nc = (seq_t *)realloc(a_cache_c, new_cap * sizeof(seq_t));
    if (!nc) abort();
    a_cache_c = nc;
    a_cap_c = new_cap;
}
#endif

/* ---------- C++ implementation ---------- */
#ifdef __cplusplus
static std::vector<seq_t> a_cache_cpp;  /* dynamic vector for C++ build */

static void init_cache_cpp(void)
{
    if (a_cache_cpp.empty()) {
        a_cache_cpp.reserve(16);
        a_cache_cpp.push_back((seq_t)1);
        a_cache_cpp.push_back((seq_t)6);
        a_cache_cpp.push_back((seq_t)16);
        a_cache_cpp.push_back((seq_t)36);
        a_cache_cpp.push_back((seq_t)76);
    }
}
#endif

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* Index 0..4 are immediate base cases */
    if (n < 5) {
#ifndef __cplusplus
        ensure_cache_c(n);
        return a_cache_c[(size_t)n];
#else
        init_cache_cpp();
        return a_cache_cpp[(size_t)n];
#endif
    }

#ifndef __cplusplus
    /* C implementation: use static array */
    ensure_cache_c(n);

    /* compute any missing terms up to n */
    for (size_t i = a_size_c; i <= (size_t)n; ++i) {
        seq_t val =
            5 * a_cache_c[i-1]
          - 10 * a_cache_c[i-2]
          + 10 * a_cache_c[i-3]
          - 5  * a_cache_c[i-4]
          + 2  * a_cache_c[i-5];
        a_cache_c[i] = val;
    }
    a_size_c = (size_t)n + 1;
    return a_cache_c[(size_t)n];
#else
    /* C++ implementation: use std::vector */
    init_cache_cpp();

    /* compute any missing terms up to n using push_back */
    size_t current_size = a_cache_cpp.size();
    for (size_t i = current_size; i <= (size_t)n; ++i) {
        seq_t val =
            5 * a_cache_cpp[i-1]
          - 10 * a_cache_cpp[i-2]
          + 10 * a_cache_cpp[i-3]
          - 5  * a_cache_cpp[i-4]
          + 2  * a_cache_cpp[i-5];
        a_cache_cpp.push_back(val);
    }
    return a_cache_cpp[(size_t)n];
#endif
}
