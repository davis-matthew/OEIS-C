/*
 * A105173  Numbers k such that either k*(k+1)/4 + 1 or k*(k+1)/4 - 1 or both
 *          are primes.
 * https://oeis.org/A105173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that * t = k·(k+1)/4 satisfies
 *            t‑1 or t+1 (or both) is prime. (Only * k with k≡0 or 3 (mod 4)
 *            are considered.)
 * Range:     u8 n <= 8, u16 n <= 607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime_uint64.h"

seq_t a(idx_t n)
{
    /* Static state – allows calls in arbitrary order. */
    static seq_t *vec = NULL;          /* array of found ks          */
    static idx_t  sz  = 0;             /* number of stored ks        */
    static size_t cap = 0;             /* current allocation size    */
    static idx_t  last = 0;            /* last k examined            */

    if (n == 0)          /* OEIS offset is 1; ignore n==0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (sz < n) {
        /* next candidate k */
        last++;

        /* k must satisfy k ≡ 0 or 3 (mod 4) for k(k+1)/4 to be integral. */
        if ((last & 3) != 0 && (last & 3) != 3)
            continue;

        /* Compute t = k*(k+1)/4; use 128‑bit arithmetic for safety. */
        unsigned __int128 prod = (unsigned __int128)last * (last + 1);
        seq_t t = (seq_t)(prod >> 2);  /* cast to seq_t */

        /* Test primality of t-1 and t+1; the helpers only accept uint64_t. */
        bool ok = false;
        uint64_t tt = (uint64_t)t;     /* t stays within 64‑bit for the tested range */
        if (tt > 1 && isPrime_uint64(tt - 1))
            ok = true;
        if (isPrime_uint64(tt + 1))
            ok = true;

        if (ok) {
            /* Grow the array if necessary. */
            if (sz == cap) {
                size_t newCap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(vec, newCap * sizeof(seq_t));
                if (!tmp)
                    exit(1);   /* allocation failure */
                vec = tmp;
                cap = newCap;
            }
            vec[sz++] = (seq_t)last;
        }
    }

    return vec[n - 1];
}
