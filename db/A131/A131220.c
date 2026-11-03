/*
 * A131220  a(n) is the least semiprime > a(n-1) whose digits do not appear in
 *          a(n-1).
 * https://oeis.org/A131220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses memoisation and a search for the next
   semiprime whose digits do not appear in the previous term. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Helper: test whether a number is a semiprime (product of two primes). */
static bool is_semiprime(seq_t n)
{
    if (n < 4) return false;            /* 0,1,2,3 are not semiprime */
    seq_t temp = n;
    int factors = 0;
    for (uint64_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        while (temp % (seq_t)p == 0) {
            ++factors;
            temp /= (seq_t)p;
            if (factors > 2) return false;  /* more than two prime factors */
        }
    }
    if (temp > 1) ++factors;             /* remaining prime factor */
    return factors == 2;
}

/* Check that all digits of `x` are not present in `used`. */
static bool digits_ok(seq_t x, const bool used[10])
{
    uint8_t buf[64];          /* larger buffer to handle large seq_t values */
    size_t len;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        if (used[buf[i]]) return false;
    return true;
}

/* Main sequence function with static memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset, safe value */

    /* Static cache: 1‑based indexing; cache[0] unused. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* currently stored terms (max index) */
    static size_t cache_cap = 0;  /* allocated array size */

    if (cache_len < n) {
        /* Allocate or grow the cache as needed. */
        if (cache == NULL) {
            ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache_len = 0;
        } else if (cache_cap < (size_t)(n + 1)) {
            ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        }

        /* Compute missing terms in order. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {                 /* a(1) = 4 */
                cache[1] = (seq_t)4;
                continue;
            }
            seq_t prev = cache[i - 1];

            /* Record digits that appear in prev. */
            bool used_digits[10] = { false };
            uint8_t buf[64];
            size_t len;
            get_digits(prev, buf, &len);
            for (size_t j = 0; j < len; ++j)
                used_digits[buf[j]] = true;

            /* Find the next semiprime > prev with disjoint digits. */
            seq_t cand = prev + (seq_t)1;
            while (true) {
                if (digits_ok(cand, used_digits) && is_semiprime(cand)) {
                    cache[i] = cand;
                    break;
                }
                cand = cand + (seq_t)1;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
