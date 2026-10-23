/*
 * A180617  Sum of divisors of the product of two consecutive primes.
 * https://oeis.org/A180617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A180617: Sum of divisors of the product of two consecutive primes.
 *  Formula: a(n) = σ(p_n · p_{n+1}) = (1 + p_n)(1 + p_{n+1})   for n ≥ 1
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */

/* static cache of consecutive primes */
static uint64_t *prime_cache = NULL;
static size_t    prime_count = 0;

/* enlarge the prime cache so that it contains at least `needed` primes */
static void
ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;

    /* simple upper bound for the `needed`‑th prime: < 20 * n for n ≤ 10⁴ */
    idx_t   limit = needed * 20;
    uint64_t *primes = NULL;
    size_t   cnt = 0;

    /* keep increasing the limit until we have enough primes */
    while (1) {
        build_primes_upto(limit, &primes, &cnt);
        if ((idx_t)cnt >= needed)
            break;
        free(primes);                        /* discard old list */
        limit *= 2;                           /* double the limit */
    }

    free(prime_cache);                       /* release old cache */
    prime_cache = primes;
    prime_count = cnt;
}

/*  Sequence function  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* offset is 1 */

    /* Need p_n and p_{n+1} → at least n+1 primes */
    ensure_primes(n + 1);

    uint64_t pn     = prime_cache[n - 1];      /* p_n  */
    uint64_t pnext  = prime_cache[n];         /* p_{n+1} */
    seq_t  res = (seq_t)(pn + 1) * (seq_t)(pnext + 1);

    return res;
}
