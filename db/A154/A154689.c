/*
 * A154689  Numbers n such that sigma_0(n-sigma_0(n))= sigma_0(n).
 * https://oeis.org/A154689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 942 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154689: numbers n such that d(n - d(n)) = d(n)  */
/* Requirements: public wrapper a(idx_t n) returning seq_t   */

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

seq_t a(idx_t n)
{
    /* 1‑based index – offset is 1 */
    if (n == 0) return (seq_t)0;

    static seq_t *vals = NULL;      /* stored sequence terms   */
    static size_t sz   = 0;          /* number of terms stored  */
    static size_t cap  = 0;          /* current allocation cap  */
    static uint64_t next = 1;        /* next integer to test    */

    /* generate terms until we have at least 'n' terms */
    while (sz < (size_t)n)
    {
        uint64_t m = next++;

        /* compute d(m) */
        uint64_t d_m = num_divisors(m);

        /* require m > d(m) so that m - d(m) > 0 (unsigned wrap avoided) */
        if (m > d_m)
        {
            uint64_t m_minus = m - d_m;
            /* test the defining property */
            if (num_divisors(m_minus) == d_m)
            {
                /* ensure space for another term */
                ensure_capacity(sz + 1, &cap, OEISC_VPP(&vals), sizeof(seq_t));
                vals[sz++] = (seq_t)m;
            }
        }
    }

    /* return the nth term (1‑based) */
    return vals[n - 1];
}
