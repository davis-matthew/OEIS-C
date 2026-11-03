/*
 * A045467  Primes congruent to {4, 6} mod 7.
 * https://oeis.org/A045467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 954 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime congruent to 4 or 6 (mod 7) – computed by incremental primality tests */

/* the function returns the n‑th prime whose value is 4 or 6 modulo 7
   (main sequence starting at n = 1).  The implementation stores all
   primes found so far in a dynamically resized array and extends it
   only as needed.  The search continues from the last candidate that
   was examined.  All indices are 1‑based.  The function is fully
   thread‑safe except for the static storage that keeps the cache. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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

/* External function provided by the test harness */
seq_t a(idx_t n)
{
    /* Static cache for the sequence values */
    static seq_t *primes = NULL;
    static size_t len      = 0;         /* number of primes stored */
    static size_t cap      = 0;         /* capacity of the cache   */
    static idx_t  last     = 2;         /* last number examined    */

    /* n is 1‑based: return 0 for n == 0, which is outside the
       official offset of the sequence. */
    if (n == 0)
        return 0;

    /* Generate new primes until we have reached the required index */
    while (len < n)
    {
        /* Move to the next candidate */
        last++;

        /* Check primality and the residue condition */
        if (is_prime(last) &&
            ((last % 7 == 4) || (last % 7 == 6)))
        {
            /* Grow the cache if necessary */
            ensure_capacity(len + 1, &cap, OEISC_VPP(&primes),
                            sizeof(seq_t));

            /* Store the found prime */
            primes[len++] = (seq_t)last;
        }
    }

    /* Return the requested element (1‑based index) */
    return primes[n - 1];
}
