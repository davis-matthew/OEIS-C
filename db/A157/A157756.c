/*
 * A157756  Integers n such that by splitting their digits in two equal parts,
 *          the difference between the square of the first part and the second
 *          part eventually reaches 0 by iterating the process. If length is
 *          odd, split (length+1)/2, (length-1)/2.
 * https://oeis.org/A157756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157756: integers n such that, by splitting the decimal digits into a left part (the first ⌈d/2⌉ digits) and a right part (the remaining ⌊d/2⌋ digits), computing left² – right, and iterating this process, the value eventually reaches 0. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* Test whether a number satisfies the A157756 property */
static bool is_good(seq_t x)
{
    seq_t cur = x;
    /* visited array to detect cycles */
    const idx_t MAX_VIS = 64;
    seq_t visited[MAX_VIS];
    idx_t vis_sz = 0;

    while (1) {
        if (cur == 0)
            return true;

        /* cycle detection */
        idx_t i;
        for (i = 0; i < vis_sz; ++i)
            if (visited[i] == cur)
                return false;

        if (vis_sz == MAX_VIS)
            return false;

        visited[vis_sz++] = cur;

        /* split the number */
        int d = num_digits(cur);          /* number of decimal digits */
        idx_t m = (idx_t)(d / 2);        /* floor(d/2) */
        seq_t powten = pow10(m);         /* 10^m */

        seq_t left  = cur / powten;
        seq_t right = cur % powten;
        seq_t left2 = left * left;

        if (left2 < right)         /* would become negative */
            return false;

        cur = left2 - right;
    }
}

/* Cached generation of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0 */
        return 0;

    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static seq_t candidate   = 0;   /* next integer to test */

    if (n <= cache_size)
        return cache[n - 1];

    while (cache_size < n) {
        if (is_good(candidate)) {
            ensure_capacity(cache_size + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_size++] = candidate;
        }
        candidate++;
    }
    return cache[n - 1];
}
