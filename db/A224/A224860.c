/*
 * A224860  Numbers n such that 90*n + 59 and 90*n + 61 are twin prime.
 * https://oeis.org/A224860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer n such that 90*n+59 and 90*n+61 are
 *            primes
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n == 0)
        return 0;           /* offset is 1; n==0 is outside the sequence */

    if ((idx_t)cache_size >= n)
        return cache[n - 1];

    seq_t start = 0;
    if (cache_size > 0)
        start = cache[cache_size - 1] + 1;

    while ((idx_t)cache_size < n) {
        seq_t x = start++;
        idx_t p1 = 90u * (idx_t)x + 59;
        idx_t p2 = 90u * (idx_t)x + 61;
        if (is_prime(p1) && is_prime(p2)) {
            ensure_capacity(cache_size + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_size++] = x;
        }
    }
    return cache[n - 1];
}
