/*
 * A201108  Primes of the form 2n^3+3.
 * https://oeis.org/A201108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*(n-1)^3+3)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 145, u64 n <= 2700,
 *            u128 n <= 2700, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth prime of the form 2k^3+3.  An internal cache of
 *  found primes is kept so that the function works correctly for
 *  calls in any order.
 */

#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /*  Cache of primes already found.  */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /*  Last 'k' value that was tested (starting with k = 0).  */
    static idx_t k  = 0;

    if (n < 1) return 0;      /*  Undefined for index below 1.  */

    /*  Generate primes until we have at least n of them.  */
    while (cache_len < n) {
        seq_t kseq = (seq_t)k;
        seq_t val  = kseq * kseq * kseq;   /*  k^3  */
        val = val * ((seq_t)2) + (seq_t)3; /*  2*k^3 + 3  */

        if (isPrimeGeneric(val)) {
            grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }
        ++k;
    }

    return cache[n - 1];
}
