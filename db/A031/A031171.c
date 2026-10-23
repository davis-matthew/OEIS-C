/*
 * A031171  a(n) = prime(n+9) - prime(n).
 * https://oeis.org/A031171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+9) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached list of primes that the function uses. */
static seq_t *primeList = NULL;   /* array of primes, each of type seq_t   */
static size_t primeCount = 0;      /* number of primes stored              */
static idx_t  sieveLimit = 0;     /* limit used for the last sieve run    */

/* Rebuild the prime array up to a new limit.  */
static void regenerate_primes(idx_t limit)
{
    /* Simple Eratosthenes sieve on a byte array. */
    uint8_t *isPrime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!isPrime) {
        /* Allocation failed – leave the old list unchanged. */
        return;
    }

    for (idx_t i = 0; i <= limit; ++i) isPrime[i] = 1;
    if (limit >= 0) isPrime[0] = 0;
    if (limit >= 1) isPrime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (isPrime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                isPrime[m] = 0;
        }
    }

    /* Count primes to allocate the list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) ++cnt;

    seq_t *list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!list) {
        free(isPrime);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) list[pos++] = (seq_t)i;

    free(isPrime);

    /* Replace old list if present. */
    if (primeList) free(primeList);
    primeList   = list;
    primeCount  = cnt;
    sieveLimit  = limit;
}

/* Ensure that at least `need` primes are available. */
static void ensure_primes(idx_t need)
{
    if (primeCount >= (size_t)need)
        return;

    idx_t limit = sieveLimit;
    if (limit < 1000) limit = 1000;

    while (primeCount < (size_t)need) {
        limit *= 2;
        regenerate_primes(limit);
    }
}

/* Sequence function for A031171. */
seq_t a(idx_t n)
{
    if (n < 1) /* offset is 1; behavior below is irrelevant */
        return 0;

    idx_t need = n + 9;          /* need at least prime(n+9) */
    ensure_primes(need);

    /* prime(n) is at index n-1 (0‑based) */
    seq_t p_n  = primeList[n - 1];
    seq_t p_n9 = primeList[n + 8];    /* n+9 becomes n+8 in 0‑based */

    return p_n9 - p_n;
}
