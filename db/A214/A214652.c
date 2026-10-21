/*
 * A214652  a(n) = (a(n-1) + a(n-4))/gcd(a(n-1), a(n-4)) with a(1) = a(2) =
 *          a(3) = a(4) = 1.
 * https://oeis.org/A214652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 66, u32 n <= 151, u64 n <= 320,
 *            u128 n <= 726, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=a(4)=1; for n≥5
   a(n) = (a(n-1) + a(n-4)) / gcd(a(n-1), a(n-4))  */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cache[0] unused; cache[1..computed] hold a(1)..a(computed) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated slots (bytes of seq_t) */
    static idx_t computed = 0;      /* highest index computed so far */

    if (n == 0)                     /* sequence starts at index 1 */
        return (seq_t)0;

    /* enlarge allocation if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            seq_t prev1 = cache[i - 1];
            seq_t prev4 = cache[i - 4];
            seq_t g = gcd(prev1, prev4);
            cache[i] = (prev1 + prev4) / g;
        }
    }
    computed = n;
    return cache[n];
}
