/*
 * A068674  Numbers which yield primes when a 3 is prefixed or appended.
 * https://oeis.org/A068674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A068674 – numbers n such that both 3 ∘ n and n∘3 are prime. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Cache of found terms.  cache[0] corresponds to a(1). */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static size_t  cap    = 0;      /* capacity of cache    */
    static size_t  count  = 0;      /* number of cached terms */
    static idx_t   last_checked = 0;/* highest integer already examined */

    if (n <= count)           /* value already known */
        return cache[n - 1];

    if (!cache) {              /* initialise cache */
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    idx_t x = last_checked + 1;      /* next integer to test */

    while (count < n) {              /* generate terms until we reach n */
        seq_t xf  = (seq_t)x;               /* cast for arithmetic */
        int    d  = digit_len(xf);          /* decimal digits of x   */
        seq_t pref = (seq_t)3 * pow10((idx_t)d) + xf;
        seq_t suff = xf * (seq_t)10 + (seq_t)3;

        if (is_prime_big(pref) && is_prime_big(suff)) {
            if (count == cap) {               /* grow cache if needed */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[count++] = xf;              /* store found term */
        }
        ++x;
    }

    last_checked = x - 1;          /* remember highest tested integer */
    return cache[n - 1];
}
