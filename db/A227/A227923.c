/*
 * A227923  Number of ways to write n = x + y (x, y > 0) such that 6*x-1 is a
 *          Sophie Germain prime and {6*y-1, 6*y+1} is a twin prime pair.
 * https://oeis.org/A227923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) of positive integers with x+y=n
 *            such that 6*x-1 is a Sophie Germain prime and {6*y-1, 6*y+1} is
 *            a twin prime pair
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple sieve stored statically; size grows with maximum n requested. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for prime sieve */
static uint8_t *prime = NULL;      /* 1 if prime, 0 otherwise */
static size_t  prime_limit = 0;    /* maximum pre‑computed value in prime[] */

/* ensure that prime[] is sieved up to at least 'limit' (inclusive) */
static void ensure_prime_sieve(idx_t limit)
{
    size_t l = (size_t)limit;
    if (l <= prime_limit)
        return;                     /* already sufficient */

    uint8_t *newprime = (uint8_t *)calloc(l + 1, 1);
    if (!newprime)
        exit(EXIT_FAILURE);         /* grave error, abort */

    /* mark all as prime initially */
    for (size_t i = 2; i <= l; ++i)
        newprime[i] = 1;

    /* sieve */
    for (size_t i = 2; i * i <= l; ++i) {
        if (newprime[i]) {
            for (size_t j = i * i; j <= l; j += i)
                newprime[j] = 0;
        }
    }

    /* replace old sieve */
    free(prime);
    prime = newprime;
    prime_limit = l;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* no positive x,y can sum to 1 */
        return (seq_t)0;

    /* Maximum number needed in prime sieve: 12*n - 1 (for 12*x-1) and
       6*n + 1 (for 6*y+1).  We take 12*n+1 for safety. */
    ensure_prime_sieve(12 * n + 1);

    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;

        size_t t  = (size_t)(6 * x - 1);   /* 6*x-1  */
        size_t tt = (size_t)(12 * x - 1);  /* 12*x-1 */
        size_t u  = (size_t)(6 * y - 1);   /* 6*y-1  */
        size_t v  = (size_t)(6 * y + 1);   /* 6*y+1  */

        if (prime[t] && prime[tt] && prime[u] && prime[v])
            ++count;
    }

    return count;
}
