/*
 * A216509  Primes which cannot be written in the form a^2 + 6*b^2.
 * https://oeis.org/A216509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes which cannot be written in the form a^2 + 6*b^2
   (i.e. primes congruent to 5, 11, 13, 17, 19, 23 modulo 24, together with 2 and 3). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
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
    /* Static data retained between calls */
    static seq_t *terms = NULL;
    static size_t term_cap = 0;
    static idx_t term_cnt = 0;          /* number of terms already computed */
    static uint64_t next_candidate = 2; /* next integer to test for primality */

    /* return already computed term if available */
    if (n <= term_cnt) {
        return terms[n - 1];
    }

    /* generate terms until we reach the requested index */
    while (term_cnt < n) {
        /* find the next prime ≥ next_candidate */
        uint64_t p = next_candidate;
        for (;;) {
            if (p == 2 || ((p & 1U) && is_prime_seq((seq_t)p))) {
                break; /* p is prime */
            }
            ++p;     /* try next integer */
        }

        /* check congruence condition */
        if (p == 2 || p == 3 ||
            (p % 24ULL == 5ULL) ||
            (p % 24ULL == 11ULL) ||
            (p % 24ULL == 13ULL) ||
            (p % 24ULL == 17ULL) ||
            (p % 24ULL == 19ULL) ||
            (p % 24ULL == 23ULL)) {
            /* store the term */
            ensure_capacity(term_cnt + 1, &term_cap,
                            OEISC_VPP(&terms), sizeof(seq_t));
            terms[term_cnt++] = (seq_t)p;
        }

        /* prepare for next search */
        next_candidate = p + 1;
    }

    /* now the requested term is available */
    return terms[n - 1];
}
