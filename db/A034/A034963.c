/*
 * A034963  Sums of four consecutive primes.
 * https://oeis.org/A034963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the sum of the four consecutive primes starting at index
 *            n (1‑based). Example: a(7) = 17 + 19 + 23 + 29 = 88
 * Range:     u8 n <= 16, u16 n <= 1898, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *primes = NULL;      /* array of primes, 1‑based index ignored */
static size_t primes_count = 0;      /* number of primes stored */
static bool primes_initialized = false;

/* Estimate a sieving upper bound for the first `cnt` primes.
   For cnt ≥ 6 a known upper bound is
       p_n < n (log n + log log n)
   which is safely over‑approximated by 20 *n for n ≤ 10⁶.
   For very small n we use a hard‑coded limit.                */
static size_t estimate_limit(size_t cnt)
{
    if (cnt < 6)
        return 15; /* contains first 6 primes (2,3,5,7,11,13)       */
    return cnt * 20; /* rough upper bound, safe for cnt up to tens of millions */
}

/* Build a simple sieve of Eratosthenes up to `limit`
   and store the primes in the static array `primes`.     */
static void build_primes_up_to(size_t limit)
{
    /* Allocate a sieve mask.  Use explicit cast to silence C++ errors.            */
    bool *mask = (bool *)calloc((limit + 1), sizeof(bool));
    if (!mask)
        exit(EXIT_FAILURE);

    /* 0 and 1 are not prime */
    for (size_t i = 2; i <= limit; ++i)
        mask[i] = true;

    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (!mask[i])
            continue;
        for (size_t j = i * i; j <= limit; j += i)
            mask[j] = false;
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (mask[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes)
    {
        free(mask);
        exit(EXIT_FAILURE);
    }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (mask[i])
            new_primes[pos++] = (uint64_t)i;

    free(mask);

    /* Replace the old array if it already existed.          */
    if (primes)
        free(primes);
    primes = new_primes;
    primes_count = cnt;
    primes_initialized = true;
}

/* Ensure that at least `need` primes are available in the static array. */
static void ensure_primes(size_t need)
{
    if (!primes_initialized || primes_count < need)
    {
        size_t limit = estimate_limit(need);
        build_primes_up_to(limit);
    }
}

/* Public function computing A034963. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but safe return */

    /* We need primes from index n to n+3  (1‑based).                             */
    size_t required = (size_t)n + 3;
    ensure_primes(required);

    seq_t sum = 0;
    for (idx_t i = 0; i < 4; ++i)
        sum += (seq_t)primes[(size_t)(n - 1 + i)];
    return sum;
}
