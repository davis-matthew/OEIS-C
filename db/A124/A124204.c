/*
 * A124204  Numbers k such that 20*k + 1 is prime.
 * https://oeis.org/A124204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124204: a(n) is the n-th integer k such that 20*k+1 is prime
 *  (OEIS offset 1).  The function builds a static cache of found values
 *  so that calls may come in any order.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found k's          */
    static size_t  cap   = 0;     /* allocated capacity         */
    static size_t  size  = 0;     /* number of elements stored  */
    static idx_t   next_k = 1;    /* next candidate k to test   */

    if (n == 0) {
        return (seq_t)0;          /* offset is 1, below not used */
    }

    /* enlarge the cache until it contains at least n elements */
    if (size < (size_t)n)
    {
        size_t need = n;
        if (cap < need) {
            size_t new_cap = cap ? cap << 1 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = seq_realloc(cache, cap, new_cap);
            if (!tmp) {
                return (seq_t)0;  /* allocation failed */
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* find additional k values until we have n terms */
        while (size < need) {
            idx_t num = (idx_t)20 * next_k + 1;   /* 20*k+1 */
            if (is_prime(num)) {
                cache[size++] = (seq_t)next_k;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
