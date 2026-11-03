/*
 * A098768  Deficient numbers whose reversal is an abundant number.
 * https://oeis.org/A098768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9763 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Deficient numbers whose decimal reversal is abundant.
   a(n) is the nth integer that is deficient (sum of proper divisors < n)
   and whose decimal reversal is abundant (sum of proper divisors > reversal). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t size = 0;       /* number of stored terms */
    static size_t cap = 0;        /* cache capacity */
    static idx_t next_candidate = 1; /* next integer to test */

    /* Grow cache until we have at least n terms */
    while (size < n) {
        idx_t cand = next_candidate;

        /* Check if cand is deficient */
        seq_t sigma_cand = sigma_of(cand);          /* sum of divisors (incl. cand) */
        seq_t proper_cand = sigma_cand - cand;      /* sum of proper divisors */
        if (proper_cand < cand) {                    /* deficient? */
            /* Compute reversal and test for abundance */
            seq_t rev = reverse_digits((seq_t)cand);
            /* If reversal does not fit in idx_t, skip this candidate */
            if (rev <= (seq_t)UINT64_MAX) {
                idx_t rev_i = (idx_t)rev;
                seq_t sigma_rev = sigma_of(rev_i);
                seq_t proper_rev = sigma_rev - rev_i;
                if (proper_rev > rev_i) {            /* abundant? */
                    /* Append to cache */
                    ensure_capacity(size + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
                    cache[size++] = (seq_t)cand;
                }
            }
        }

        next_candidate = cand + 1;  /* next integer to test */
    }

    return cache[n - 1];
}
