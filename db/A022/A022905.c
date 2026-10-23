/*
 * A022905  a(n) = M(n) + m(n) for n >= 2, where M(n) = max{ a(i) + a(n-i): i
 *          = 1..n-1 }, m(n) = min{ a(i) + a(n-i): i = 1..n-1 }.
 * https://oeis.org/A022905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 1 + a(floor(n/2)) + a(ceil(n/2)) for n>1, a(1) =
 *            1
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 293, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 4, a(3) = 10, ... (OEIS A022905) */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic cache: cache[0] is unused; cache[1]..cache[cap-1] contain
   computed terms. A value of 0 indicates "not yet computed". */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache can store indices up to n (inclusive).  New elements
   are initialized to 0.  Safe in both C and C++ (explicit cast). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) {
        return;                            /* already enough */
    }
    size_t new_cap = (size_t)(n + 1);
    /* realloc may return NULL if memory cannot be allocated; abort to
       avoid undefined behaviour. */
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    /* Initialise new slots to 0. */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        p[i] = (seq_t)0;
    }
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* n < 1 is undefined */

    ensure_cache_size(n);

    /* Base case */
    if (cache[1] == (seq_t)0) {
        cache[1] = (seq_t)1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already known */

        idx_t half      = i / 2;              /* floor(i/2)          */
        idx_t ceil_half = i - half;           /* ceil(i/2) = (i+1)/2 */

        cache[i] = cache[i - 1]
                 + (seq_t)1
                 + cache[half]
                 + cache[ceil_half];
    }

    return cache[n];
}
