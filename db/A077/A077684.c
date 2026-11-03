/*
 * A077684  Squarefree numbers beginning with 8.
 * https://oeis.org/A077684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth squarefree integer whose first decimal digit is 8
 * Range:     u8 n <= 6, u16 n <= 672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9683 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches sequentially for numbers that both start with 8
   and are squarefree, caching results for repeated calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* State for the incremental generation of the sequence */
static seq_t base_seq   = 1;  /* 10^k */
static seq_t cur_seq    = 8;  /* current candidate */
static seq_t limit_seq  = 8;  /* upper bound of current 10^k block */

/* Generate terms until we have at least target terms cached */
static void generate_until(size_t target) {
    ensure_capacity(target, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    while (cache_len < target) {
        if (cur_seq > limit_seq) {           /* move to next 10^k block */
            base_seq = base_seq * 10;        /* base *= 10 */
            limit_seq = 9 * base_seq - 1;    /* 9*10^k - 1 */
            cur_seq = 8 * base_seq;          /* 8*10^k */
        }
        if (is_squarefree(cur_seq)) {
            cache[cache_len++] = cur_seq;
        }
        cur_seq += 1;                        /* next candidate */
    }
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;   /* offset starts at 1; behaviour for n < 1 is undefined */
    }
    size_t target = (size_t)n;   /* convert to a native size_t for indexing */
    if (cache_len < target) {
        generate_until(target);
    }
    return cache[target - 1];
}
