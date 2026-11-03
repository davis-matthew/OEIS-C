/*
 * A028885  Numbers k such that k^2 - 8 is prime.
 * https://oeis.org/A028885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that k^2 - 8 is prime a(n) is the n-th such k
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t next_k = 5;          /* starting candidate */

    while (count < n) {
        /* Grow storage if needed */
        if (count + 1 > cap) {
            ensure_capacity(count + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            if (!cache) return 0;    /* allocation failed */
        }

        seq_t k = (seq_t)next_k;
        seq_t candidate = k * k - (seq_t)8;
        if (is_prime((idx_t)candidate)) {
            cache[count++] = k;
        }
        next_k++;
    }
    return cache[n - 1];
}
