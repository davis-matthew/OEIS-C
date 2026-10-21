/*
 * A231474  Primes whose base-3 representation is also the base-5
 *          representation of a prime.
 * https://oeis.org/A231474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 237, u32 n <= 62, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that the base‑3 digits of p interpreted as a
 * base‑5 number are also prime.  The function builds the sequence
 * lazily and caches all results, so calls with arbitrary index order
 * are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"            /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* ---  static cache  ------------------------------------------ */
    static seq_t *cache = NULL;           /* array of found primes   */
    static size_t cache_len = 0;          /* number stored so far   */
    static idx_t checked = 1;             /* last integer inspected */

    /* ---  find further terms until we have n of them -------------- */
    while (cache_len < (size_t)n)
    {
        /* advance to next candidate */
        checked++;                        /* start from 2 onward */
        if (!is_prime(checked))
            continue;                    /* not a prime → skip */

        /* convert candidate to base‑3 digits (least significant first) */
        idx_t  t    = checked;
        idx_t  digs[128];
        size_t len  = 0;

        while (t > 0)
        {
            digs[len++] = t % 3;
            t           = t / 3;
        }
        if (len == 0) digs[len++] = 0;    /* handle the (impossible) case 0 */

        /* interpret those digits as a base‑5 number (most significant
           first) */
        seq_t q = 0;
        for (size_t i = 0; i < len; ++i)
            q = q * 5 + (seq_t)digs[len - 1 - i];

        /* both numbers must be prime */
        if (is_prime_big(q))
        {
            /* grow the cache if necessary */
            if (cache_len == 0)
                cache = (seq_t*)malloc(sizeof(seq_t));
            else
                cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));

            cache[cache_len++] = checked;
        }
    }

    return cache[n - 1];
}
