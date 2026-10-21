/*
 * A090922  Least multiple of n such that the n-th concatenation is a multiple
 *          of n+1.
 * https://oeis.org/A090922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the least multiple of n such that the concatenation of
 *            a(1)…a(n) is divisible by n+1. The sequence is computed
 *            incrementally; for each n the smallest m = n·t that satisfies
 *            (C(n-1)·10^{len(m)} + m) mod (n+1) == 0 is chosen, where C(n-1)
 *            is the concatenation of the first n–1 terms
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for both C11 and C++17.  The type
   seq_t may be a builtin unsigned integer type or an arbitrary‑
   precision class; idx_t is always an unsigned integer type such
   as uint64_t.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
static size_t computed_max = 0;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t computed_max = 0;
#endif

/* modular power of 10: 10^len (mod mod).  Mod small (<= n+1). */
static unsigned long long pow10_mod(unsigned long long len, unsigned long long mod) {
    unsigned long long result = 1 % mod;
    for (unsigned long long i = 0; i < len; ++i)
        result = (result * 10ULL) % mod;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1; return 0 for safety */

    /* Ensure the cache can hold indices up to n. */
#if defined(__cplusplus)
    if (cache.size() <= (size_t)n)
        cache.resize(n + 1);          /* enlarge to index n (0 unused) */
#else
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n + 16;          /* some slack */
        seq_t *new_cache = realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);
        cache = new_cache;
        cache_size = new_size;
    }
#endif

    /* Compute terms from computed_max+1 up to n. */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        unsigned long long mod = (unsigned long long)i + 1ULL;   /* n+1 */
        unsigned long long prefix_mod = 0ULL;

        /* remainder of the concatenation of a[1]…a[i-1] modulo (i+1). */
        for (size_t j = 1; j < i; ++j) {
            seq_t a_j = cache[j];
            unsigned long long len_j = (unsigned long long)digit_len(a_j);
            unsigned long long pow10_j = pow10_mod(len_j, mod);
            unsigned long long a_j_mod = (unsigned long long)(a_j % (seq_t)mod);
            prefix_mod = (prefix_mod * pow10_j + a_j_mod) % mod;
        }

        /* Search for the least multiple of i that makes the full
           concatenation divisible by (i+1). */
        seq_t value = 0;
        for (idx_t t = 1; ; ++t) {
            seq_t m = (seq_t)i * (seq_t)t;            /* candidate multiple */
            unsigned long long len_m = (unsigned long long)digit_len(m);
            unsigned long long pow10_m = pow10_mod(len_m, mod);
            unsigned long long m_mod = ((unsigned long long)i * (unsigned long long)t) % mod;
            unsigned long long check = (prefix_mod * pow10_m + m_mod) % mod;
            if (check == 0ULL) {
                value = m;
                break;
            }
        }

        cache[i] = value;
    }

    computed_max = (size_t)n;

    return cache[n];
}
