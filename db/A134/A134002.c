/*
 * A134002  Positive integers n such that n(n+5)=a(a+5)+b(b+5) is solvable in
 *          positive integers.
 * https://oeis.org/A134002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134002
   a(n) = the n-th positive integer k such that k^2 + (k+5)^2 is composite.
   The function scans successive positive integers, tests the composite
   condition using is_prime_big, and caches the found terms for fast
   subsequent lookups. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t sz   = 0;       /* current number of stored terms */
    static size_t cap  = 0;       /* current capacity of `cache` */
    static idx_t next  = 1;       /* next integer to test */

    /* Grow cache until it holds at least n terms. */
    while (sz < (size_t)n) {
        seq_t k = (seq_t)next;                      /* candidate value */
        seq_t kk = k * k;                           /* k^2 */
        seq_t k5 = (k + 5) * (k + 5);               /* (k+5)^2 */
        seq_t sum = kk + k5;                        /* k^2 + (k+5)^2 */

        if (!is_prime_big(sum)) {                   /* composite -> term */
            if (sz == cap) {                        /* increase capacity */
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = alloc_seq_array(newcap);
                if (!tmp) abort();
                for (size_t i = 0; i < sz; ++i)
                    tmp[i] = cache[i];
                if (cache)
                    free_seq_array(cache);
                cache = tmp;
                cap = newcap;
            }
            cache[sz++] = k;
        }
        ++next;
    }

    return cache[n - 1];
}
