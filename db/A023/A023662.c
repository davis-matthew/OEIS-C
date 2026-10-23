/*
 * A023662  Convolution of odd numbers and primes.
 * https://oeis.org/A023662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-1} prime(n-i) * (2*i+1)
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    static uint32_t *primes = NULL;   /* cache of the first primes */
    static size_t   primes_cnt = 0;   /* number of cached primes   */

    if (n == 0)
        return 0;

    /* Ensure that we have at least n primes cached. */
    if ((size_t)n > primes_cnt) {
        if (primes != NULL)
            free(primes);
        size_t cnt = 0;
        uint32_t *tmp = NULL;
        /* sieve_first_n_primes fills tmp with the first n primes */
        sieve_first_n_primes(n, &tmp, &cnt);
        primes = tmp;
        primes_cnt = cnt;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t p = (seq_t)primes[k - 1];                     /* prime(k)   */
        seq_t w = ((seq_t)2 * n) - ((seq_t)2 * k) + (seq_t)1;  /* 2*(n-k)+1 */
        sum += p * w;
    }
    return sum;
}
