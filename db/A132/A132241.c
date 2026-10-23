/*
 * A132241  Twin primes congruent to {11, 13} mod 30.
 * https://oeis.org/A132241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime p such that p ≡ 11 or 13 (mod 30) and
 *            either p+2 or p−2 is also prime (i.e., p is a member of a
 *            twin‑prime pair)
 * Range:     u8 n <= 10, u16 n <= 584, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache of already found terms */
    static seq_t *cache = NULL;
    static idx_t len   = 0;   /* number of stored terms */
    static idx_t cap   = 0;   /* capacity of the cache array */

    /* current integer candidate to test for primality */
    static idx_t cur   = 2;   /* start from 2 */

    if (n == 0)
        return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    /* generate terms until we have at least n of them */
    while (len < n) {
        if (is_prime(cur)) {
            idx_t mod = cur % 30ULL;
            /* test for twin‑prime condition */
            if ((mod == 11ULL && is_prime(cur + 2)) ||
                (mod == 13ULL && is_prime(cur - 2))) {

                /* ensure cache capacity */
                if (len == cap) {
                    cap = (cap == 0) ? 16 : cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!tmp)
                        abort();               /* out of memory */
                    cache = tmp;
                }
                cache[len++] = (seq_t)cur;   /* store the found prime */
            }
        }
        ++cur;   /* advance to the next integer to test */
    }

    return cache[n - 1];
}
