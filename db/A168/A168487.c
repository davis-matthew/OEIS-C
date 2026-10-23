/*
 * A168487  Primes of the form 100n^3 + 27.
 * https://oeis.org/A168487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 5, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th prime of the form 100*m^3 + 27. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state: cache of computed primes, last m examined,
       and capacity information. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  last_m = 0;

    /* Compute terms until we have at least n entries. */
    while (len < n)
    {
        last_m++;                      /* Next candidate m */
        seq_t m   = (seq_t)last_m;      /* Promote to seq_t for arithmetic */
        seq_t val = m * m;              /* m^2 */
        val *= m;                       /* m^3 */
        val *= 100;                     /* 100*m^3 */
        val += 27;                      /* 100*m^3 + 27 */

        if (is_prime_big(val))
            append_candidate(&cache, &len, &cap, val);
    }

    return cache[n-1];   /* a(1) is the first element */
}
