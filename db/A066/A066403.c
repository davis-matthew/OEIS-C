/*
 * A066403  From expansion of Belyi function for cube.
 * https://oeis.org/A066403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066403
   Offset: 0,  a(0)=0, a(1)=1, a(2)=46, a(3)=1347, a(4)=32220, a(5)=686661
   Generating function:  G(x) = x*(1+x)^4 / (1-14*x+x^2)^3
   Recurrence derived from (1-14x+x^2)^3 * G(x) = x*(1+x)^4:
   For n >= 6 :
       a(n) = 42*a(n-1) - 591*a(n-2) + 2828*a(n-3)
              - 591*a(n-4) + 42*a(n-5) - a(n-6)
   (The N(x) term contributes only for n=1..5, as listed above.)  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;   /* array storing a(0)...a(current) */
    static size_t   cap    = 0;   /* allocated capacity */
    static idx_t    len    = 0;   /* number of terms currently stored (len > 0 if cache allocated) */

    /* initialise cache with the first 6 terms, when the function is first called */
    if (len == 0) {
        cap = 64;                 /* initial capacity, can grow later */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)46;
        cache[3] = (seq_t)1347;
        cache[4] = (seq_t)32220;
        cache[5] = (seq_t)686661;
        len = 6;
    }

    /* extend cache up to index n */
    while (len <= n) {
        /* grow if necessary */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                /* allocation failed; for safety, return 0 */
                return (seq_t)0;
            }
            cache = new_cache;
            cap = new_cap;
        }

        idx_t i = len;  /* compute a(i) */
        /* compute using the recurrence:
           a(i) = 42*a(i-1) - 591*a(i-2) + 2828*a(i-3)
                  - 591*a(i-4) + 42*a(i-5) - a(i-6)
         */
        seq_t term = 0;
        /* positive contributions */
        term += (seq_t)42 * cache[i-1];
        term += (seq_t)2828 * cache[i-3];
        term += (seq_t)42 * cache[i-5];
        /* negative contributions */
        term -= (seq_t)591 * cache[i-2];
        term -= (seq_t)591 * cache[i-4];
        term -= cache[i-6];
        cache[i] = term;
        len++;
    }

    return cache[n];
}
