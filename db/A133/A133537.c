/*
 * A133537  Sum of sixth powers of two consecutive primes.
 * https://oeis.org/A133537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n)^6 + p(n+1)^6
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(k) denotes the k-th prime (p(1)=2). This function returns the
 * n-th term of OEIS sequence A133537, with OEIS offset 1. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behaviour below OEIS offset is irrelevant */

    /* lazy cache of the first primes needed */
    static uint32_t *primes = NULL;
    static size_t prime_count = 0;

    /* ensure we have at least n+1 primes cached */
    if ((size_t)(n + 1) > prime_count) {
        size_t need = (size_t)(n + 1);
        uint32_t *tmp = NULL;
        size_t len = 0;
        sieve_first_n_primes((idx_t)need, &tmp, &len);
        if (primes)
            free(primes);
        primes = tmp;
        prime_count = len;   /* should equal need */
    }

    /* compute sixth powers of the two consecutive primes */
    seq_t p1 = (seq_t)primes[n - 1];
    for (int i = 1; i < 6; ++i)
        p1 *= primes[n - 1];

    seq_t p2 = (seq_t)primes[n];
    for (int i = 1; i < 6; ++i)
        p2 *= primes[n];

    return p1 + p2;
}
