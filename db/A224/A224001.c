/*
 * A224001  Number of 3 X n 0..2 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7), with a(1)=27, a(2)=157, a(3)=476,
 *            a(4)=1144, a(5)=2403, a(6)=4614, a(7)=8291, a(8)=14141,
 *            a(9)=23109; a(n) = a(n) for n>9
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 89, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 178 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *cache = NULL;       /* sequence cache (0‑based: cache[0] == a(1)) */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static size_t cache_len = 0;      /* number of computed terms */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, guard for safety */

    /* ensure we have all terms up to n */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        ensure_capacity(new_len, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* initial 9 terms (indices 0..8 correspond to a(1)..a(9)) */
        static const seq_t init[9] = {
            27,    /* a(1) */
            157,   /* a(2) */
            476,   /* a(3) */
            1144,  /* a(4) */
            2403,  /* a(5) */
            4614,  /* a(6) */
            8291,  /* a(7) */
            14141, /* a(8) */
            23109  /* a(9) */
        };

        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 9) {
                cache[i] = init[i];
            } else {
                seq_t sum_pos;
                seq_t sum_sub;

                /* sum_pos = 7*a(i-1) + 35*a(i-3) + 21*a(i-5) + a(i-7) */
                sum_pos = mul_small(cache[i-1], 7);
                sum_pos += mul_small(cache[i-3], 35);
                sum_pos += mul_small(cache[i-5], 21);
                sum_pos += cache[i-7];

                /* sum_sub = 21*a(i-2) + 35*a(i-4) + 7*a(i-6) */
                sum_sub = mul_small(cache[i-2], 21);
                sum_sub += mul_small(cache[i-4], 35);
                sum_sub += mul_small(cache[i-6], 7);

                cache[i] = sum_pos - sum_sub;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
