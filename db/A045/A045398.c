/*
 * A045398  Primes congruent to {0, 1, 3, 5, 6} mod 7.
 * https://oeis.org/A045398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045398: primes congruent to {0,1,3,5,6} (mod 7)
   (i.e. all primes except those ≡ 2 or 4 (mod 7), plus 7 itself)
*/
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t cache_cap = 0;     /* allocated size */
    static size_t cache_len = 0;     /* number of terms currently stored */
    static idx_t last_checked = 2;   /* last number examined for primality */

    if (n == 0)                  /* undefined for offset 1, return 0 as dummy */
        return (seq_t)0;

    /* Return cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        last_checked++;          /* next candidate */

        if (is_prime(last_checked)) {
            idx_t m = last_checked % 7;
            if (m == 0 || m == 1 || m == 3 || m == 5 || m == 6) {
                /* Ensure capacity for new element */
                if (cache_len >= cache_cap)
                    grow_seq(&cache, &cache_cap, cache_len + 1);

                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }

    return cache[n - 1];
}
