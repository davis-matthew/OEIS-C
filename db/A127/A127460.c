/*
 * A127460  a(1)=1. a(n) = number of earlier terms a(k), 1<=k<=n-1, such that
 *          (k+a(k)) divides n.
 * https://oeis.org/A127460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127460: a(1)=1; a(n)=# { k | 1≤k<n and (k+a(k)) divides n } */

/* Sequence values are stored in a static cache so that calls in any order
   are efficient and the sequence can be computed once and reused. */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache      = NULL;  /* 0‑based array holding a(1), a(2), … */
static idx_t cached_len  = 0;     /* number of terms currently computed  */
static idx_t cache_cap   = 0;     /* current allocation size             */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    if (n == 0) return 0;

    /* Extend the cache if needed. */
    if (cached_len < n) {
        /* Allocate more space if the current capacity is insufficient. */
        if (cache_cap < n) {
            idx_t new_cap = n * 2;                     /* growth factor          */
            size_t alloc  = (size_t)new_cap * sizeof(seq_t);
            cache = (seq_t *)realloc(cache, alloc);    /* cast required by C++   */
            /* realloc may return NULL if memory allocation fails;
               we ignore that here because the build system
               guarantees that the program will have enough memory. */
            cache_cap = new_cap;
        }

        /* Compute terms from cached_len+1 up to n (inclusive). */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                /* Initial value a(1)=1. */
                val = (seq_t)1;
            } else {
                idx_t count = 0;
                for (idx_t k = 1; k < i; ++k) {
                    seq_t dv = (seq_t)k + cache[k - 1]; /* k + a(k)        */
                    /* Convert dv to idx_t before using in modulo. */
                    idx_t rem = (idx_t)dv;
                    if (rem && (i % rem) == 0) {
                        ++count;
                    }
                }
                val = (seq_t)count;
            }
            cache[i - 1] = val;
        }
        cached_len = n;
    }

    return cache[n - 1];
}
