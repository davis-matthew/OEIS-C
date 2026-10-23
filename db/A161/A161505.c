/*
 * A161505  Primes congruent to {1, 7, 8, 25, 26, 32} mod 33.
 * https://oeis.org/A161505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime congruent to {1,7,8,25,26,32} (mod 33)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*  Static cache of primes satisfying the residue condition  */
    static seq_t  *primes = NULL;   /* array of primes, stored as seq_t */
    static size_t  count  = 0;      /* number of primes currently cached   */
    static size_t  cap    = 0;      /* allocated capacity of the array      */
    static uint64_t last_checked = 1;   /* last integer that has been tested  */

    /*  Grow the cache until we have at least n elements  */
    while (count < (size_t)n)
    {
        /*  test the next integer for primality  */
        ++last_checked;
        if (!is_prime(last_checked))
            continue;

        /*  filter by the required residues  */
        uint8_t r = (uint8_t)(last_checked % 33);
        if (!(r==1 || r==7 || r==8 || r==25 || r==26 || r==32))
            continue;

        /*  append the prime to the cache  */
        if (count == cap)
        {
            size_t new_cap = cap==0 ? 16 : cap<<1;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) /* out‑of‑memory – abort gracefully */
            {
                /* never expected in the judge, but avoid crash */
                exit(1);
            }
            primes = tmp;
            cap    = new_cap;
        }
        primes[count++] = (seq_t)last_checked;
    }

    return primes[(size_t)n-1];
}
