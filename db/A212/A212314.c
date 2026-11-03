/*
 * A212314  Numbers m such that B(m^3) = 3*B(m), where B(m) is the binary
 *          weight of m (A000120).
 * https://oeis.org/A212314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers m such that B(m^3) = 3*B(m), where B(x) is
 *            popcount(x) (A000120)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1223, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9942 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities for dynamic allocation */
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

/* Helper: popcount for arbitrary-width seq_t */
static inline unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state for memoized generation */
    static seq_t *terms = NULL;
    static size_t capacity = 0;
    static size_t len = 0;
    static idx_t next_candidate = 0;   /* next integer to test */

    if (n == 0) /* undefined in OEIS, but guard against caller misuse */
        return 0;

    /* generate terms until we have at least n entries */
    while (len < n)
    {
        seq_t candidate = 0;

        /* find next term */
        for (;; ++next_candidate)
        {
            seq_t sm = (seq_t)next_candidate;
            unsigned bc = popcount_seq_t(sm);

            seq_t m2 = sm * sm;
            seq_t m3 = m2 * sm;
            unsigned bc3 = popcount_seq_t(m3);

            if (bc3 == 3 * bc)
            {
                candidate = sm;
                ++next_candidate;   /* start next search from next integer */
                break;
            }
        }

        /* ensure we have space in the array */
        if (len >= capacity)
        {
            size_t newcap = capacity ? capacity << 1 : 16;
            ensure_capacity(len + 1, &capacity, OEISC_VPP(&terms), sizeof(seq_t));
        }

        terms[len++] = candidate;
    }

    return terms[n - 1];
}
