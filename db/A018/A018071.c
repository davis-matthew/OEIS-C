/*
 * A018071  Expansion of 1/((1-3x)(1-8x)(1-12x)).
 * https://oeis.org/A018071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=23, a(2)=373; * for n ≥ 3: a(n) = 23*a(n‑1) –
 *            156*a(n‑2) + 288*a(n‑3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 190 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache   = NULL;   /* dynamic cache of terms */
static size_t cap     = 0;      /* current allocated capacity */
static size_t computed = 0;     /* number of terms already stored */

/* Return the n‑th term of A018071. */
seq_t a(idx_t n)
{
    size_t i = (size_t)n;              /* cast index for array access */

    /* Initialise cache for the three base cases on first call. */
    if (cache == NULL) {
        size_t init = 3;
        ensure_capacity(init, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)373;
        computed = 3;
    }

    /* Return known value directly. */
    if (i < computed) {
        return cache[i];
    }

    /* Grow cache if required. */
    if (i >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= i) {
            new_cap <<= 1;
        }
        ensure_capacity(new_cap, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Compute all missing terms up to i using the linear recurrence. */
    for (size_t j = computed; j <= i; ++j) {
        seq_t term =
            (seq_t)23 * cache[j-1] -
            (seq_t)156 * cache[j-2] +
            (seq_t)288 * cache[j-3];
        cache[j] = term;
    }
    computed = i + 1;

    return cache[i];
}
