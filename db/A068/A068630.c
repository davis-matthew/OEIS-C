/*
 * A068630  a(0)=0; a(n) = n + a(n-1) if gcd(n,a(n-1)) = 1 or a(n-1) = 0; a(n)
 *          = a(n-1) - n if n divides a(n-1), otherwise a(n) = a(n-1).
 * https://oeis.org/A068630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   for n>0:
       if gcd(n,a(n-1))==1 or a(n-1)==0   then a(n)=a(n-1)+n;
       else if n divides a(n-1)           then a(n)=a(n-1)-n;
       else                                 a(n)=a(n-1); */

#include <stdint.h>
#include <stdlib.h>

/* Euclidean algorithm for seq_t (unsigned integer types, including __int128). */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cap       = 0;   /* capacity of cache in elements */
    static idx_t max_comp  = 0;   /* highest index computed so far */

    /* Initial allocation: store a(0) = 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = 0;
        cap = 1;
        max_comp = 0;
    }

    /* Resize the cache if n is beyond current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? (size_t)cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cur;
        seq_t si = (seq_t)i;

        if (prev == 0 || seq_gcd(prev, si) == 1) {
            cur = prev + si;
        } else if (prev % si == 0) {
            cur = prev - si;
        } else {
            cur = prev;
        }
        cache[i] = cur;
    }
    max_comp = n;

    return cache[n];
}
