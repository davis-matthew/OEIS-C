/*
 * A079279  a(n)=n for n<=3; for n>3, a(n) is next integer sharing common
 *          factors with 1 or 2 of previous 3 terms.
 * https://oeis.org/A079279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = next integer > a(n-1) that shares a common factor with *
 *            exactly 1 or 2 of the previous 3 terms; * a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=4. * The sequence is built incrementally, memoizing
 *            all generated terms * so that any call order is supported
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms            */
    static seq_t *cache   = NULL;   /* array of computed a(i)      */
    static idx_t  size    = 0;      /* number of terms in cache    */
    static idx_t  cap     = 0;      /* capacity of allocated array */

    /* initialise cache with first four terms if needed   */
    if (!cache) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */
        cache[0] = 1;  /* a(1) */
        cache[1] = 2;  /* a(2) */
        cache[2] = 3;  /* a(3) */
        cache[3] = 4;  /* a(4) */
        size = 4;
    }

    /* generate terms up to requested index n                */
    while (size < n) {
        seq_t candidate = cache[size - 1] + 1;  /* start just above last term */

        /* search for the next integer that shares a factor with
         * exactly 1 or 2 of the previous 3 terms                   */
        for (;;) {
            int shares = 0;
            if (gcd(candidate, cache[size - 3]) > 1) shares++;
            if (gcd(candidate, cache[size - 2]) > 1) shares++;
            if (gcd(candidate, cache[size - 1]) > 1) shares++;

            if (shares == 1 || shares == 2)
                break;   /* candidate satisfies rule */
            ++candidate;
        }

        /* ensure capacity and add new term to cache */
        grow_seq(&cache, &cap, size + 1);
        cache[size] = candidate;
        ++size;
    }

    return cache[n - 1];  /* a(n) is stored at index n-1 */
}
