/*
 * A015817  Numbers n such that phi(n + 1) | sigma(n) for n congruent to 1
 *          (mod 3).
 * https://oeis.org/A015817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m such that m ≡ 1 (mod 3) and φ(m + 1)
 *            divides σ(m)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 57 terms through n = 99 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence by
   incrementally testing candidates 1 mod 3.  The
   functions sigma_of and phi are provided by the
   utils library.  Results of the search are memoized
   so that the function works correctly for arbitrary
   calling order.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/totient.h"
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

seq_t a(idx_t n)
{
    /* Static data for memoization */
    static seq_t *values = NULL;   /* array of found terms      */
    static size_t count   = 0;     /* number of stored terms    */
    static size_t cap     = 0;     /* current allocation size   */
    static idx_t  cand    = 1;     /* next candidate 1 mod 3    */

    if (n == 0)            /* undefined for n=0, return 0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (count < (size_t)n) {
        /* Compute σ(cand) and φ(cand + 1).  The
           functions return seq_t values, and the
           modulo operation is defined for seq_t. */
        seq_t sigma_val = sigma_of(cand);
        seq_t phi_val   = totient(cand + (idx_t)1);

        if (phi_val != 0 && (sigma_val % phi_val) == 0) {
            /* Store this term */
            ensure_capacity(count + 1, &cap, OEISC_VPP(&values), sizeof(seq_t));
            values[count++] = (seq_t)cand;
        }

        /* Next candidate that is ≡1 (mod 3) */
        cand += 3;
    }

    /* Return the requested term (1‑based index). */
    return values[n - 1];
}
