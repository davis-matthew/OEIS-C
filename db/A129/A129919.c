/*
 * A129919  a(n) is the smallest prime of the form b*prime(n+1) + prime(n)
 *          with b > 0.
 * https://oeis.org/A129919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 222, u32 n <= 873, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129919: a(n) = smallest prime of the form b*prime(n+1)+prime(n) with b>0
   (prime(1)=2).  This implementation generates primes on demand and tests
   the candidate primes using the is_prime_big() utility. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/is_prime_big.h"        /* is_prime_big(candidate) */

static uint64_t *primes = NULL;   /* array of primes up to the current limit */
static size_t    prime_cnt = 0;   /* number of primes stored */
static idx_t     last_limit = 0;  /* last sieve upper bound used */

/* Ensure we have at least the (n+1)-st prime available. */
static void
ensure_primes(idx_t n)
{
    /* We need primes[0 .. n] so that primes[n] is prime(n+1). */
    size_t need = (size_t)(n + 1);
    if (prime_cnt > need)
        return;  /* already sufficient */

    /* Rough upper bound for the (n+1)-th prime. */
    idx_t new_limit = n * 20 + 10;
    if (new_limit < 100)
        new_limit = 100;

    if (new_limit <= last_limit)
        new_limit = last_limit * 2;

    if (primes != NULL)
        free(primes);  /* discard previous sieve */

    build_primes_upto(new_limit, &primes, &prime_cnt);
    last_limit = new_limit;

    /* If still not enough, keep doubling the limit. */
    while (prime_cnt <= need) {
        new_limit *= 2;
        free(primes);
        build_primes_upto(new_limit, &primes, &prime_cnt);
        last_limit = new_limit;
    }
}

/* Compute the n-th term of A129919 (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n >= 1 */

    /* Make sure primes array contains prime(n) and prime(n+1). */
    ensure_primes(n);

    /* Retrieve the required primes. */
    seq_t p_n     = (seq_t)primes[(size_t)n - 1];   /* prime(n)   */
    seq_t p_next  = (seq_t)primes[(size_t)n];       /* prime(n+1) */

    idx_t b = 1;
    for (;;) {
        seq_t candidate = (seq_t)b * p_next + p_n;
        if (is_prime_big(candidate))
            return candidate;
        ++b;
    }
}
