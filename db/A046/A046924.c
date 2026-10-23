/*
 * A046924  Number of ways to express n as p+2q; p, q = 1 or prime.
 * https://oeis.org/A046924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for each n, a(n) = number of pairs (p,q) such that p+2q=n and
 *            p,q are either 1 or primes
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static uint64_t *primes = NULL;          /* cached list of primes up to current limit */
static size_t    primes_cnt = 0;         /* number of primes in the cache */
static idx_t     primes_limit = 0;       /* maximum prime value in the cache */

/* Ensure the prime cache contains all primes ≤ max_limit. */
static void
ensure_primes_upto(idx_t max_limit)
{
    if (max_limit <= primes_limit)
        return;   /* already sufficient */

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;

    build_primes_upto(max_limit, &new_primes, &new_cnt);

    free(primes);
    primes      = new_primes;
    primes_cnt  = new_cnt;
    primes_limit = max_limit;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* safety for non‑positive indices */

    seq_t count = 0;

    /* handle q = 1 case */
    if (n >= 2) {
        idx_t p = n - 2;
        if (p == 1 || is_prime_big((seq_t)p))
            count += 1;
    }

    /* no need to test other qs if n is very small */
    if (n < 4) return count;

    /* need primes up to floor(n/2) for q */
    idx_t max_q = n >> 1;        /* n / 2 */
    ensure_primes_upto(max_q);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];
        if (q > max_q) break;

        idx_t p = n - 2 * (idx_t)q;
        if (p == 1 || is_prime_big((seq_t)p))
            count += 1;
    }

    return count;
}
