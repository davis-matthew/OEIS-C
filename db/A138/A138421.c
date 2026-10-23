/*
 * A138421  a(n) = (prime(n)^4 - prime(n)^2)/6.
 * https://oeis.org/A138421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^4 - prime(n)^2)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n) {
    /* Prime indexing is 0‑based: prime(1) is primes[0] */
    if (n < 1) return 0;

    static idx_t last_n = 0;
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (n > last_n) {
        /* Re‑sieve to obtain at least n primes */
        uint32_t *tmp = NULL;
        size_t tmpcnt = 0;
        sieve_first_n_primes(n, &tmp, &tmpcnt);
        free(primes);
        primes   = tmp;
        prime_cnt = tmpcnt;
        last_n   = n;
    }

    uint32_t p = primes[n - 1];
    seq_t p_t = (seq_t)p;
    seq_t p2  = p_t * p_t;   /* p^2 */
    seq_t p4  = p2 * p2;     /* p^4 */
    return (p4 - p2) / 6;
}
