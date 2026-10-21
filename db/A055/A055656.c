/*
 * A055656  Excess in exponents of powers of 2 in Euler phi of n! compared to
 *          that of n!.
 * https://oeis.org/A055656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -1 + sum_{p odd prime ≤ n} v₂(p – 1), a(1) = 0 where
 *            v₂(x) is the exponent of 2 in the factorisation of x
 * Range:     i8 n <= 348, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data for memoisation */
static uint64_t *primes = NULL;      /* array of odd primes, sorted ascending   */
static seq_t   *prefix = NULL;      /* prefix sums of v₂(p-1) for primes[i]   */
static size_t   primes_len = 0;     /* number of primes stored                */
static uint64_t max_sieved = 0;     /* largest n for which we have built data */

/* Build primes and prefix sums up to (and including) n.
   The sieve is rebuilt from scratch each time n increases.
   Since n ≤ ~10000 for the intended tests, this is inexpensive. */
static void ensure_sieve(uint64_t n)
{
    if (n <= max_sieved)
        return;

    /* free previous arrays */
    if (primes)  free(primes);
    if (prefix)  free(prefix);

    /* allocate new arrays big enough to hold all odd primes ≤ n */
    size_t alloc_size = n + 1;                 /* safe over‑estimate */
    primes  = (uint64_t *)malloc(alloc_size * sizeof(uint64_t));
    prefix  = (seq_t *)malloc(alloc_size * sizeof(seq_t));
    primes_len = 0;

    /* simple sieve of Eratosthenes with dynamic allocation */
    bool *comp = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!comp) /* allocation failure – give up */
        return;

    seq_t running_sum = 0;

    for (uint64_t i = 2; i <= n; ++i) {
        if (!comp[i]) {
            /* i is prime */
            if (i >= 3) {            /* ignore prime 2 */
                /* compute v₂(i‑1) */
                uint64_t x = i - 1;
                seq_t v = 0;
                while ((x & 1ULL) == 0ULL) {
                    x >>= 1;
                    ++v;
                }
                running_sum += v;
                primes[primes_len] = i;
                prefix[primes_len] = running_sum;
                ++primes_len;
            }
            /* mark multiples */
            if (i * i <= n) {
                for (uint64_t j = i * i; j <= n; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
    max_sieved = n;
}

/* number of primes ≤ n, binary search */
static size_t count_primes_leq(uint64_t n)
{
    size_t lo = 0, hi = primes_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;        /* count of primes ≤ n */
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* cast to native unsigned */
    if (N < 1)
        return 0;                      /* undefined for n < 1, return 0 */

    if (N == 1)
        return 0;                      /* special case a(1) = 0 */

    /* ensure we have primes up to N */
    ensure_sieve(N);

    size_t cnt = count_primes_leq(N);
    seq_t sum = (cnt == 0) ? 0 : prefix[cnt - 1];

    /* a(n) = -1 + sum_{p odd prime ≤ n} v₂(p-1) */
    return sum - 1;
}
