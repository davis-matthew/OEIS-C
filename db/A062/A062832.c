/*
 * A062832  Numbers k such that k and k+2 have the same number of divisors.
 * https://oeis.org/A062832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the number of divisors of k
 *            equals the number of divisors of k+2 (OEIS A062832)
 * Range:     u8 n <= 54, u16 n <= 9024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9894 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/num_divisors.h"
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

static seq_t *seq_cache = NULL;      /* array of found terms */
static size_t seq_len   = 0;         /* number of terms stored so far */
static size_t seq_cap   = 0;         /* current allocation capacity */
static uint64_t next_k   = 1;        /* next integer to test */

static void add_term(uint64_t k) {
    if (seq_len == seq_cap) {
        /* grow the cache to hold at least one more element */
        ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq_cache), sizeof(seq_t));
    }
    seq_cache[seq_len++] = (seq_t)k;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for the offset, guard against accidental call */

    /* keep generating terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t d1 = num_divisors(next_k);
        uint64_t d2 = num_divisors(next_k + 2);
        if (d1 == d2) {
            add_term(next_k);
        }
        next_k++;
    }

    return seq_cache[(size_t)n - 1];
}
