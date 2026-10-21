/*
 * A091953  a(1)=1, a(2n)=1+a(n)(mod 2); a(2n+1)=2*a(2n)+1.
 * https://oeis.org/A091953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n)=1+(a(n)&1); a(2n+1)=2*a(2n)+1 (values ∈ {1,2,3,5})
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based static cache – grows only forward */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)            /* undefined, safety value */
        return 0;

    if (n > cache_size) {                  /* need to fill cache up to n */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                       (new_size + 1) * sizeof(seq_t));
        if (!tmp)
            abort();                        /* out of memory */
        cache = tmp;

        /* compute new entries */
        if (cache_size == 0)                /* initialize first element */
            cache[1] = 1;

        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if ((i & 1U) == 0) {            /* i is even: a(2k)   */
                seq_t parity = ((cache[i >> 1] & 1) + 1);
                cache[i] = parity;          /* 1 or 2               */
            } else {                        /* i is odd : a(2k+1) */
                cache[i] = (cache[i - 1] << 1) + 1; /* 3 or 5 */
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
