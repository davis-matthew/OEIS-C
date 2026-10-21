/*
 * A071854  a(n) = (number of distinct prime factors in C(2n,n)) - pi(n).
 * https://oeis.org/A071854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω(C(2n,n)) – π(n) (ω = number of distinct prime factors)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* provides build_primes(idx_t , idx_t **, size_t *) */

/* Cached list of primes up to the largest limit requested so far. */
static idx_t *prime_list = NULL;
static size_t  prime_cnt  = 0;
static idx_t   prime_lim  = 0;

/* Ensure that prime_list contains all primes ≤ limit.  */
static void
ensure_primes(idx_t limit)
{
    if (prime_lim >= limit)
        return;               /* already sufficient */

    /* Free the old list (if any) and build a new one. */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_lim  = 0;

    build_primes(limit, &prime_list, &prime_cnt);
    prime_lim = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1 – return 0 for n=0 */
        return 0;

    idx_t two_n = 2 * n;
    ensure_primes(two_n);

    /* Count distinct prime factors of C(2n,n). */
    int distinct = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= two_n; ++i)
    {
        idx_t p = prime_list[i];
        idx_t exp = 0;
        idx_t power = p;
        while (power <= two_n)
        {
            exp += (two_n / power) - 2 * (n / power);
            if (power > two_n / p)          /* prevent overflow and stop */
                break;
            power *= p;
        }
        if (exp > 0)
            ++distinct;
    }

    /* Compute π(n) – number of primes ≤ n. */
    int pi = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n; ++i)
        ++pi;

    /* The difference is non‑negative for all n≥1. */
    return (seq_t)(distinct - pi);
}
