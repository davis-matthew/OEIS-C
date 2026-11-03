/*
 * A183106  Numbers k such that sum of palindromic divisors of k (A088000(k))
 *          is palindromic.
 * https://oeis.org/A183106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   ipow_i32, is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the nth integer k such that the sum of its palindromic
   divisors is a palindromic number.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

seq_t a(idx_t n) {
    /* Handle out‑of‑range indices (behavior below offset is unspecified). */
    if (n == 0) return 0;

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_candidate = 1; /* next integer to test */

    /* Grow the cache as needed */
    if (n > cache_len) {
        ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        while (cache_len < n) {
            seq_t k = (seq_t)next_candidate;
            seq_t sum = 0;
            seq_t sqrtk = floor_sqrt(k);
            uint64_t sqrt_lim = (uint64_t)sqrtk;

            for (uint64_t i = 1; i <= sqrt_lim; ++i) {
                if ((k % (seq_t)i) == 0) {
                    seq_t d1 = (seq_t)i;
                    seq_t d2 = k / d1;

                    if (is_palindrome_base(d1, 10)) sum += d1;

                    if (d1 != d2 && is_palindrome_base(d2, 10)) sum += d2;
                }
            }

            if (is_palindrome_base(sum, 10)) {
                cache[cache_len++] = k;
            }

            ++next_candidate;
        }
    }

    return cache[n - 1];
}
