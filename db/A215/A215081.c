/*
 * A215081  Smallest number k such that the difference between the greatest
 *          prime divisor and the smallest prime divisor of k equals n, or 0
 *          if there is no such k.
 * https://oeis.org/A215081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the difference between the greatest
 *            * prime divisor and the smallest prime divisor of k equals n; *
 *            a(0) = 2. For n > 0, a(n) = p*(p+n) where p is the smallest
 *            prime * such that p and p+n are also prime. If no such k exists,
 *            return 0. * * The implementation searches fo...
 * Range:     u8 n <= 29, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

static uint64_t *g_primes = NULL;      /* array of primes up to LIMIT */
static size_t   g_prime_cnt = 0;       /* number of primes in the array */
static bool     g_primes_built = false;
#define LIMIT 20000000ULL          /* 20 million – plenty for n≤10000 */

/* Build the prime list once, on first use. */
static void init_prime_list(void)
{
    if (!g_primes_built) {
        build_primes_upto(LIMIT, &g_primes, &g_prime_cnt);
        g_primes_built = true;
    }
}

seq_t a(idx_t n)
{
    /* Normalise n; it is already an idx_t. */
    if (n == 0) return (seq_t)2;

    /* n odd → only possible pair involves the even prime 2. */
    if (n & 1ULL) {
        seq_t q = (seq_t)((uint64_t)2 + (uint64_t)n);
        if (is_prime_big(q))
            return (seq_t)2 * q;
        else
            return (seq_t)0;
    }

    /* n even → search odd primes p. */
    init_prime_list();

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (p <= 2) continue;          /* skip the even prime 2 */

        seq_t q = (seq_t)p + (seq_t)n;
        if (is_prime_big(q))
            return (seq_t)p * q;
    }

    /* No suitable pair found within the sieve limit. */
    return (seq_t)0;
}
