/*
 * A104774  a(1) = 1, a(2) = 2, a(3) = 0, a(4) = 4, and for n > 4, if n is an
 *          even number, a(n) = a(n/2) - (1 if a(n/2) is not zero), otherwise
 *          if n is an odd semiprime p*q (with q >= p), then a(n) = p+q,
 *          otherwise 0.
 * https://oeis.org/A104774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence definition of OEIS sequence A104774:
    a(1)=1, a(2)=2, a(3)=0, a(4)=4.
    For n>4:
        if n is even:   a(n)=a(n/2) - (a(n/2)!=0 ? 1 : 0)
        else if n is an odd semiprime p*q (p<=q): a(n)=p+q
        else:           a(n)=0
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;            /* safety for out‑of‑range */

    /* enlarge cache if needed */
    if (cache_size < n) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failure */
        cache = new_cache;

        /* compute terms from cache_size+1 to new_size */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            seq_t val = 0;

            /* initial values */
            if (i == 1)           val = 1;
            else if (i == 2)      val = 2;
            else if (i == 3)      val = 0;
            else if (i == 4)      val = 4;
            /* even n > 4 */
            else if ((i & 1) == 0) {
                seq_t half = cache[(i / 2) - 1];
                val = half;
                if (half != 0) val -= 1;
            }
            /* odd n > 4 */
            else {
                bool found = false;
                seq_t sum = 0;
                /* search for a factor p such that i = p*q with p,q primes */
                for (idx_t p = 3; p <= i / p; p += 2) {   /* p <= sqrt(i) */
                    if (i % p == 0) {
                        idx_t q = i / p;
                        if (is_prime(p) && is_prime(q)) {
                            sum = (seq_t)(p + q);
                            found = true;
                            break;
                        }
                    }
                }
                val = found ? sum : 0;
            }

            cache[i - 1] = val;
        }

        cache_size = new_size;
    }

    return cache[n - 1];
}
