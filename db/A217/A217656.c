/*
 * A217656  Primes p such that p = 361 + 420*k for some k.
 * https://oeis.org/A217656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence enumerates primes of the form 361 + 420·k. a(n) is
 *            the n‑th prime in that arithmetic progression
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes in the progression 361 + 420·k */
    static seq_t *primes   = NULL;   /* array of primes           */
    static size_t count    = 0;      /* number of primes stored   */
    static size_t capacity = 0;      /* current capacity of arr   */
    static idx_t next_k    = 0;      /* next k to test            */

    if (n < 1) return 0;            /* undefined, return 0 */

    /* Generate primes until we have at least n entries */
    while (count < n) {
        /* p = 361 + 420·k  */
        seq_t p = 361 + ((seq_t)420 * (seq_t)next_k);
        if (isPrimeGeneric(p))
            append_candidate(&primes, &count, &capacity, p);
        ++next_k;
    }

    return primes[(size_t)(n - 1)];   /* 0‑based indexing */
}
