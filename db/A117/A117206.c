/*
 * A117206  Squarefree positive integers k such that 2*k-1 is also squarefree.
 * https://oeis.org/A117206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A117205(n)+1)/2
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117206: squarefree positive integers k such that 2*k-1 is also squarefree.
   The function enumerates natural numbers, checks the squarefree property, and
   stores found terms in a static cache so further calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for computed values */
static seq_t   *cache   = NULL;   /* array of terms */
static size_t   cap     = 0;      /* current capacity of cache   */
static idx_t    count   = 0;      /* number of terms stored      */
static idx_t    next_k  = 1;      /* next candidate k to test   */

/* Test whether an unsigned 64‑bit integer is square‑free.
   Uses trial division up to sqrt(x). */
static bool is_sqfree_uint64(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        uint64_t pp = p * p;
        if (x % pp == 0)
            return false;         /* divisible by p^2 */
    }
    return true;
}

/* Ensure the cache has room for at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap)
        return;
    size_t old_cap = cap;
    if (cap == 0)
        cap = 4;
    while (cap < new_cap)
        cap <<= 1;                 /* double until big enough */
    seq_t *new_ptr = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_ptr) {
        abort();    /* Out of memory */
    }
    cache = new_ptr;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for zero index, return 0 */

    /* Compute terms until we have at least n of them */
    while (count < n) {
        for (idx_t k = next_k; ; ++k) {
            /* Test k and 2*k-1 for square‑freeness */
            uint64_t uk   = (uint64_t)k;
            uint64_t u2k1 = 2 * uk - 1;

            if (is_sqfree_uint64(uk) && is_sqfree_uint64(u2k1)) {
                /* Store term */
                ensure_capacity(count + 1);
                cache[count++] = (seq_t)k;
                next_k = k + 1;
                break;              /* go back to while(count < n) */
            }
        }
    }
    return cache[n - 1];
}
