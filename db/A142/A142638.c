/*
 * A142638  Primes congruent to 52 mod 55.
 * https://oeis.org/A142638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 52 (mod 55)
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n < 1 */

    /* static cache of found primes in this congruence class */
    static seq_t *list = NULL;
    static size_t  cap   = 0;   /* allocation capacity */
    static size_t  count = 0;   /* number of primes stored */

    /* if the requested term is already cached, return it */
    if (n <= count)
        return list[n - 1];

    /* start searching from the current candidate:
       the first such prime is 107 (107 ≡ 52 mod 55) */
    idx_t cand = 107;
    if (count > 0)
        cand = (idx_t)list[count - 1] + (idx_t)55;  /* next residue class */

    /* extend the cache until it contains n terms */
    while (count < n)
    {
        if (is_prime(cand))          /* primality test for idx_t values */
        {
            if (count + 1 > cap)     /* grow the cache if needed */
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();         /* allocation failed */
                list = tmp;
                cap = new_cap;
            }
            list[count++] = (seq_t)cand;
        }
        cand += (idx_t)55;           /* next number with the same residue */
    }

    return list[n - 1];
}
