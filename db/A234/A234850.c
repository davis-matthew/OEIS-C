/*
 * A234850  Primes in A014692, i.e., of the form prime(k)-k+1, for some k.
 * https://oeis.org/A234850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form prime(k) - k + 1 (the primes
 *            occurring in the sequence A014692)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static idx_t cache_size = 0;           /* number of stored terms     */
    static idx_t cache_cap   = 0;           /* allocated size            */
    static seq_t *cache      = NULL;        /* cache array               */
    static idx_t last_k      = 1;           /* next k to test            */

    /* accumulate terms until we have at least n of them */
    while (cache_size < n) {
        /* ensure sufficient capacity */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        } else if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* compute candidate value prime(k) - k + 1 */
        seq_t pk  = (seq_t)get_prime(last_k);   /* prime(k)          */
        seq_t val = pk - (seq_t)last_k + 1;     /* candidate value   */

        /* keep it if it is prime */
        if (isPrimeGeneric(val))
            cache[cache_size++] = val;

        ++last_k;   /* next k to test */
    }

    return cache[n - 1];   /* a(1) is at index 0 */
}
