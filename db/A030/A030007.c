/*
 * A030007  a(n) = (prime(n)-3)*(prime(n)-5)/8.
 * https://oeis.org/A030007
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-3)*(prime(n)-5)/8
 * Range:     u8 n <= 15, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* behaviour below the offset is irrelevant */

    /* upper bound estimate for the n-th prime */
    idx_t limit;
    if (n < 6) {
        limit = 15;                      /* guarantees primes 2,3,5,7,11,13 */
    } else {
        double dn = (double)n;
        /* nth prime <= n(ln n + ln ln n) for n >= 6 */
        double lim = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (idx_t)lim;
    }

    idx_t *primes = NULL;
    size_t count;
    build_primes(limit, &primes, &count);

    /* If the limit was too small (unlikely with the above bound) grow and rebuild */
    while (count < n) {
        limit = limit * 2 + 1;
        free(primes);
        build_primes(limit, &primes, &count);
    }

    /* primes array is 0‑based, index n-1 gives prime(n) */
    seq_t p = (seq_t)primes[n - 1];
    seq_t result = (p - 3) * (p - 5) / 8;

    free(primes);
    return result;
}
