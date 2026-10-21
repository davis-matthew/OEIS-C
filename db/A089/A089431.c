/*
 * A089431  Even-indexed terms of A089423.
 * https://oeis.org/A089431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let L(n) = lcm(1, 2, …, 2n). a(n) = L(n) if n is a power of two
 *            (including n=0), else a(n) = 2 * L(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   build_primes_upto, floor_log2_uint64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t limit = 2 * n;                      /* compute lcm up to 2n */
    /* Determine exponent of 2 */
    idx_t exp2 = floor_log2_uint64(limit);     /* floor(log2(limit)) */
    if ( (n & (n - 1)) != 0 ) {               /* n not a power of two */
        exp2 += 1;                             /* extra factor of 2 */
    }

    /* Prepare prime list up to 'limit' */
    static uint64_t *primes = NULL;            /* array of primes */
    static size_t prime_cnt = 0;               /* number of primes */
    static idx_t  primes_limit = 0;            /* maximum built limit */

    if (limit > primes_limit) {
        if (primes) free(primes);              /* free old array if any */
        build_primes_upto(limit, &primes, &prime_cnt);
        primes_limit = limit;
    }

    /* Compute product of prime powers */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && primes[i] <= limit; ++i) {
        idx_t p   = (idx_t)primes[i];
        idx_t exp = 0;

        if (p == 2) {
            exp = (idx_t)exp2;                 /* exponent of 2 */
        } else {
            /* floor(log_p(limit)) */
            idx_t val = p;
            while (val <= limit) {
                exp   += 1;
                val   *= p;
            }
        }

        seq_t pow_val = pow_seq((seq_t)p, (int)exp);
        result = result * pow_val;
    }

    return result;
}
