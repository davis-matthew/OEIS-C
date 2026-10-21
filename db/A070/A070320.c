/*
 * A070320  a(n) = Max( phi(k) : k=1..n ).
 * https://oeis.org/A070320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{ φ(k) : 1 ≤ k ≤ n } = (largest prime ≤ n) – 1 (with
 *            a(1) = 1). The function builds a growing sieve of Eratosthenes
 *            to find the largest prime ≤ n and returns that prime minus one
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static idx_t prime_limit = 0;      /* highest n that has been sieved */
static seq_t *prime_list = NULL;  /* primes ≤ prime_limit (increasing) */
static size_t prime_cnt = 0;       /* number of primes stored */

/* Ensure that all primes ≤ n are present in prime_list. */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_limit)
        return;  /* already sufficient */

    /* Allocate a sieve array marking composites.  0/false = prime candidate. */
    bool *comp = (bool *)calloc(n + 1, sizeof(bool));
    if (!comp)
        exit(EXIT_FAILURE);

    /* Classic Eratosthenes sieve up to n. */
    for (idx_t i = 2; i * i <= n; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= n; j += i)
                comp[j] = true;

    /* Count the primes to allocate the result array. */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!comp[i])
            ++new_cnt;

    /* Allocate space for the primes. */
    seq_t *new_primes = (seq_t *)malloc(new_cnt * sizeof(seq_t));
    if (!new_primes)
        exit(EXIT_FAILURE);

    /* Copy the primes into the array. */
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!comp[i])
            new_primes[idx++] = (seq_t)i;

    free(comp);

    /* Replace the old list with the new one. */
    free(prime_list);
    prime_list = new_primes;
    prime_cnt = new_cnt;
    prime_limit = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined out-of-range */
    if (n == 1)
        return (seq_t)1;            /* special case per OEIS entry */

    ensure_primes(n);

    /* Binary search for the largest prime ≤ n. */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq_t p = prime_list[lo - 1];
    return p - 1;
}
