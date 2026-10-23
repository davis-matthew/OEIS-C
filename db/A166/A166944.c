/*
 * A166944  a(1)=2; a(n) = a(n-1) + gcd(n, a(n-1)) if n is even, a(n) = a(n-1)
 *          + gcd(n-2, a(n-1)) if n is odd.
 * https://oeis.org/A166944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 2
   a(n) = a(n-1) + gcd(n, a(n-1))           if n is even
   a(n) = a(n-1) + gcd(n-2, a(n-1))         if n is odd
   (semicolons are required for valid C code)
*/
#include <stdlib.h>
#include "../utils/gcd.h"

/* External linkage */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots */
    static idx_t highest = 0;      /* highest index computed */

    /* initialise on first call */
    if (cache == NULL) {
        cache_size = 2;                 /* indices 0..1 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {            /* allocation failure */
            /* terminate program as per C conventions */
            abort();
        }
        cache[0] = 0;                   /* unused */
        cache[1] = (seq_t)2;            /* a(1) = 2 */
        highest = 1;
    }

    /* expand cache if needed */
    if (n >= (idx_t)cache_size) {
        size_t new_cap = cache_size;
        while ((size_t)n >= new_cap) {
            new_cap = (new_cap == 0) ? 2 : new_cap << 1;   /* double */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {               /* allocation failure */
            abort();
        }
        cache = tmp;
        cache_size = (idx_t)new_cap;
    }

    /* compute values up to n */
    for (idx_t i = highest + 1; i <= n; ++i) {
        seq_t addend;
        if ((i & 1) == 0) {              /* i is even */
            addend = gcd((seq_t)i, cache[i - 1]);
        } else {                         /* i is odd */
            addend = gcd((seq_t)(i - 2), cache[i - 1]);
        }
        cache[i] = cache[i - 1] + addend;
    }
    highest = n;

    return cache[n];
}
