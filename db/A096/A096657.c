/*
 * A096657  a(n) = (2^n)*a(n-1) + (2^(n-1))*a(n-2), a(0)=1, a(1)=3.
 * https://oeis.org/A096657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n)*a(n-1) + (2^(n-1))*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps two growing tables:
   a_cache[k]  = a(k)
   pow_cache[k] = 2^k
   Both tables are expanded on demand so that a() can be called
   in any order and any number of times.  The implementation is
   identical for C and for C++ (the big‑integer build uses a
   class with the usual arithmetic operators). */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* static arrays (std::vector) for C++ builds */
    static std::vector<seq_t> a_cache;
    static std::vector<seq_t> pow_cache;

    /* initialise to the two base values if this is the first call */
    if (a_cache.size() == 0) {
        a_cache.resize(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
    }
    if (pow_cache.size() == 0) {
        pow_cache.resize(1);
        pow_cache[0] = (seq_t)1;            /* 2^0 */
    }

    /* expand tables if necessary */
    size_t target = (size_t)n;

    /* grow pow_cache first */
    if (pow_cache.size() <= target) {
        size_t old = pow_cache.size();   /* last element already known */
        pow_cache.resize(target + 1);
        for (size_t i = old; i <= target; ++i) {
            if (i == 0)
                pow_cache[0] = (seq_t)1;
            else
                pow_cache[i] = pow_cache[i - 1] << 1;
        }
    }

    /* grow a_cache */
    if (a_cache.size() <= target) {
        size_t old = a_cache.size();    /* last computed index */
        a_cache.resize(target + 1);
        for (size_t i = old; i <= target; ++i) {
            if (i == 0)
                a_cache[0] = (seq_t)1;
            else if (i == 1)
                a_cache[1] = (seq_t)3;
            else
                a_cache[i] = a_cache[i - 1] * pow_cache[i]
                            + a_cache[i - 2] * pow_cache[i - 1];
        }
    }

    return a_cache[target];
#else
    /* static arrays for C builds */
    static seq_t *a_cache = NULL;
    static size_t a_len   = 0;
    static seq_t *pow_cache = NULL;
    static size_t pow_len   = 0;

    size_t target = (size_t)n;

    /* initialise base values on first call */
    if (a_len == 0) {
        a_len = 2;
        a_cache = (seq_t*)malloc(a_len * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
    }
    if (pow_len == 0) {
        pow_len = 1;
        pow_cache = (seq_t*)malloc(pow_len * sizeof(seq_t));
        pow_cache[0] = (seq_t)1;            /* 2^0 */
    }

    if (pow_len <= target) {
        size_t old = pow_len;
        pow_len = target + 1;
        pow_cache = (seq_t*)realloc(pow_cache, pow_len * sizeof(seq_t));
        for (size_t i = old; i <= target; ++i) {
            if (i == 0)
                pow_cache[0] = (seq_t)1;
            else
                pow_cache[i] = pow_cache[i - 1] << 1;
        }
    }

    if (a_len <= target) {
        size_t old = a_len;
        a_len = target + 1;
        a_cache = (seq_t*)realloc(a_cache, a_len * sizeof(seq_t));
        for (size_t i = old; i <= target; ++i) {
            if (i == 0)
                a_cache[0] = (seq_t)1;
            else if (i == 1)
                a_cache[1] = (seq_t)3;
            else
                a_cache[i] = a_cache[i - 1] * pow_cache[i]
                            + a_cache[i - 2] * pow_cache[i - 1];
        }
    }

    return a_cache[target];
#endif
}
