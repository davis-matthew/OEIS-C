/*
 * A140841  Primes of the form 210n + 13.
 * https://oeis.org/A140841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 210n + 13, 1‑based index */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Keep a cache of all primes found so far. */
    static seq_t *primes = NULL;
    static size_t sz = 0;        /* number of primes stored */
    static size_t cap = 0;       /* capacity of the array */
    static idx_t last_k = 0;     /* next k to test (k starts at 0) */

    if (n == 0) return 0; /* undefined for n=0, harmless */

    /* Generate primes until we have at least n of them. */
    while (sz < n)
    {
        seq_t candidate = 210ULL * (seq_t)last_k + 13ULL;
        if (isPrimeGeneric(candidate))
        {
            if (sz == cap)
                grow_seq(&primes, &cap, sz + 1);
            primes[sz++] = candidate;
        }
        last_k++;
    }

    return primes[n - 1];
}
