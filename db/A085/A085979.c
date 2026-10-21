/*
 * A085979  Number of 5's in decimal expansion of prime(n).
 * https://oeis.org/A085979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = count of digit '5' in the decimal representation of the
 *            nth prime
 * Range:     u8 n <= 54, u16 n <= 6840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, but guard against n==0 */

    /* Cache the list of primes up to a suitable limit. */
    static idx_t *primes = NULL;          /* array of primes (1‑based) */
    static size_t prime_cnt = 0;           /* number of primes stored */
    static idx_t last_limit = 0;           /* last sieve limit used */

    /* If the requested index is larger than we have, enlarge the sieve. */
    if (!primes || n > prime_cnt) {
        idx_t new_limit;
        if (n < 6) {
            new_limit = 15;                /* first few primes fit in small sieve */
        } else {
            /* Simple asymptotic upper bound for the nth prime:
               p_n < n (log n + log log n) for n >= 6                */
            double nd = (double)n;
            new_limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
        }

        /* Build a new prime list up to new_limit. */
        size_t cnt = 0;
        idx_t *tmp = NULL;
        build_primes(new_limit, &tmp, &cnt);

        /* Free previous list if any, store the new one. */
        if (primes) free(primes);
        primes = tmp;
        prime_cnt = cnt;
        last_limit = new_limit;
    }

    /* Retrieve the nth prime. The array is 0‑based, so use n-1. */
    seq_t prime = (seq_t)primes[n - 1];

    /* Count occurrences of the digit 5 in the decimal representation. */
    unsigned long long val = (unsigned long long)prime;   /* safe: primes fit in 64‑bit for n <= 10000 */
    seq_t count = 0;
    while (val > 0) {
        if (val % 10 == 5) ++count;
        val /= 10;
    }
    return count;
}
