/*
 * A226083  Smallest element of the set of largest prime powers p^k dividing
 *          C(2*n,n), where p is any prime factor of C(2*n,n).
 * https://oeis.org/A226083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest largest prime power dividing C(2*n,n)
 * Range:     u8 n <= 533, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = min_{p prime|C(2n,n)} p^v_p(C(2n,n))          */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

static uint64_t *prime_cache = NULL;      /* primes up to current limit   */
static size_t prime_cnt = 0;              /* number of primes in cache    */
static idx_t prime_limit = 0;             /* current maximum prime limit  */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* sequence defined for n>=1    */

    /* ensure prime table covers up to 2*n */
    idx_t max = n * 2;
    if (max > prime_limit) {
        free(prime_cache);
        build_primes_upto(max, &prime_cache, &prime_cnt);
        prime_limit = max;
    }

    seq_t best = 0;                     /* 0 indicates uninitialized   */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > (uint64_t)max) break;   /* primes beyond 2n not needed */

        /* compute exponent of p in C(2n,n) using Legendre's formula */
        idx_t exp = 0;
        for (idx_t pw = p; pw <= max; pw = pw * p) {
            idx_t term = (max / pw) - 2 * (n / pw);
            exp += term;
            if (pw > (idx_t)((uint64_t)~0 / p)) break; /* prevent overflow */
        }
        if (exp == 0) continue;         /* should not happen, skip if so */

        seq_t pow = big_pow((seq_t)p, (seq_t)exp);

        if (best == 0 || pow < best) best = pow;
    }
    return best;
}
