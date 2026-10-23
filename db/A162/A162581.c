/*
 * A162581  G.f.: A(x) = exp( 2*Sum_{n>=1} A006519(n)^2 * x^n/n ), where
 *          A006519(n) = highest power of 2 dividing n.
 * https://oeis.org/A162581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=a(n-1)+∑_{i=0}^{⌊n/2⌋} a(i)*a(⌊n/2⌋-i)
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 62, u64 n <= 200, u128 n <= 692,
 *            big any n
 * Helpers:   seq_alloc, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the functional equation A(x)=(1+x)A(x²)²/(1-x). */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_one.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence and of the convolution sums. */
    static seq_t *a_cache = NULL;          /* a_cache[i] = a(i)          */
    static size_t a_cap   = 0;              /* allocated size of a_cache  */
    static idx_t  a_max   = 0;              /* largest index computed     */

    static seq_t *c_cache = NULL;          /* c_cache[k] = ∑_{i≤k} a(i)a(k-i) */
    static size_t c_cap   = 0;              /* allocated size of c_cache  */
    static idx_t  c_max   = 0;              /* largest k with c_cache[k] computed */

    /* First call: create the caches. */
    if (a_cache == NULL) {
        a_cap   = 1;
        a_cache = seq_alloc(a_cap);
        a_cache[0] = seq_one();   /* a(0) = 1 */
        a_max   = 0;
    }

    if (c_cache == NULL) {
        c_cap   = 1;
        c_cache = seq_alloc(c_cap);
        c_cache[0] = (seq_t)1;    /* a(0)*a(0) = 1 */
        c_max   = 0;
    }

    /* Ensure capacity for the requested index. */
    if ((size_t)n >= a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = seq_realloc(a_cache, a_cap, new_cap);
        a_cap   = new_cap;
    }

    /* Ensure capacity for convolution sums up to ⌊n/2⌋. */
    idx_t k_needed = n >> 1;
    if ((size_t)k_needed >= c_cap) {
        size_t new_cap = c_cap ? c_cap * 2 : 1;
        while (new_cap <= (size_t)k_needed) new_cap <<= 1;
        c_cache = seq_realloc(c_cache, c_cap, new_cap);
        c_cap   = new_cap;
    }

    /* Compute missing terms in ascending order. */
    while (a_max < n) {
        idx_t next = a_max + 1;
        idx_t k    = next >> 1;        /* floor(next/2) */

        /* Compute convolution sum for this k if it hasn't been done. */
        if (k > c_max) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= k; ++i) {
                sum += a_cache[i] * a_cache[k - i];
            }
            c_cache[k] = sum;
            c_max = k;
        }

        /* Ensure a_cache has space for next (safety). */
        if ((size_t)next >= a_cap) {
            size_t new_cap = a_cap ? a_cap * 2 : 1;
            while (new_cap <= (size_t)next) new_cap <<= 1;
            a_cache = seq_realloc(a_cache, a_cap, new_cap);
            a_cap   = new_cap;
        }

        a_cache[next] = a_cache[next - 1] + c_cache[k];
        a_max = next;
    }

    return a_cache[n];
}
