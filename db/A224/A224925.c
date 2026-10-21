/*
 * A224925  a(n) = number of primes of the form p*q-(p+q) where p = prime(n)
 *          and q is any prime < p.
 * https://oeis.org/A224925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   an = number of primes of the form p*q-(p+q) where p = prime(n)
 *            and q is any prime < p
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper function prototypes */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/isPrimeGeneric.h"

/* Static cache for primes up to the largest index requested */
static uint64_t *primes   = NULL;   /* array of prime numbers        */
static size_t   primes_cnt = 0;     /* number of primes in 'primes' */

/* Ensure that primes[0 .. n-1] contain the first n primes */
static void
ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n)
        return;                       /* already have enough primes */

    /* Free the previous array, if any */
    if (primes != NULL) {
        free(primes);
        primes   = NULL;
        primes_cnt = 0;
    }

    /* Find an upper bound for the nth prime and generate primes up to it */
    idx_t limit = _prime_limit_for_idx(n);
    build_primes_upto(limit, &primes, &primes_cnt);
}

/* Core function: compute a(n) */
seq_t
a(idx_t n)
{
    if (n == 0)           /* safety; AEIS offset is 1 */
        return (seq_t)0;

    ensure_primes(n);

    const uint64_t p = primes[n - 1];   /* nth prime */
    seq_t count = 0;

    for (idx_t i = 0; i < n - 1; ++i) {
        const uint64_t q = primes[i];

        /* m = p*q - (p+q) */
        seq_t m = ((seq_t)p) * ((seq_t)q)
                - (((seq_t)p) + ((seq_t)q));

        if (m < 2)                /* primes are >= 2 */
            continue;

        if (isPrimeGeneric(m))
            ++count;
    }

    return count;
}
