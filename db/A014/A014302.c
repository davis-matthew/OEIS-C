/*
 * A014302  a(n) = prime(n)*(prime(n-1)-1)/2.
 * https://oeis.org/A014302
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*(prime(n-1)-1)/2
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 6719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static inline idx_t prime_upper_bound(idx_t n) {
    if (n < 6) return 15;          /* small n, safe upper bound */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (idx_t)(bound + 3.0);   /* add margin */
}

seq_t a(idx_t n) {
    if (n < 3) return 0;           /* offset is 3, below that is undefined */

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (prime_cnt < (size_t)n) {
        idx_t limit = prime_upper_bound(n);
        uint64_t *new_primes;
        size_t new_cnt;
        build_primes_upto(limit, &new_primes, &new_cnt);
        free(primes);              /* free old array if any */
        primes = new_primes;
        prime_cnt = new_cnt;
    }

    seq_t p_n   = (seq_t)primes[n - 1];   /* prime(n)   */
    seq_t p_prev = (seq_t)primes[n - 2];  /* prime(n-1) */
    seq_t result = p_n * (p_prev - 1) / 2;
    return result;
}
