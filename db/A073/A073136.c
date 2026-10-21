/*
 * A073136  a(n) = prime(n) + prime(prime(n)).
 * https://oeis.org/A073136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + prime(prime(n))
 * Range:     u8 n <= 14, u16 n <= 785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime using a simple sieve and then
   returns prime(n) + prime(prime(n)).  The prime table is
   cached between calls to avoid recomputation. */
#include <math.h>
#include <stdlib.h>

static unsigned long long *prime_cache = NULL;   /* array of primes (1‑based) */
static size_t          prime_cache_len = 0;     /* count of stored primes */

/* Estimate an upper bound for the n-th prime.
   Uses Rosser's theorem: for n ≥ 6, p_n < n (log n + log log n). */
static size_t
prime_limit_estimate(size_t n)
{
    if (n < 6) return 15;
    double d = (double)n;
    double val = d * (log(d) + log(log(d))) + 3.0;
    return (size_t)val;
}

/* Expand the prime table so it contains at least `needed` primes. */
static void
ensure_primes(size_t needed)
{
    if (prime_cache_len >= needed) return;        /* already have enough */

    /* estimate new size bound */
    size_t limit = prime_limit_estimate(needed);
    unsigned char *sieve = (unsigned char *)malloc(limit + 1);
    if (!sieve) return;               /* out of memory – caller will fail */

    /* initialize sieve */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++count;

    /* allocate (or grow) prime array */
    unsigned long long *new_cache = (unsigned long long *)
        realloc(prime_cache, count * sizeof(unsigned long long));
    if (!new_cache) {
        free(sieve);
        return;                       /* memory failure */
    }
    prime_cache = new_cache;
    prime_cache_len = count;

    /* fill prime array */
    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < count; ++i)
        if (sieve[i]) prime_cache[idx++] = i;

    free(sieve);
}

/* Return the n-th prime (1‑based).  Caller must ensure `n >= 1`. */
static inline unsigned long long
nth_prime(idx_t n)
{
    ensure_primes((size_t)n);
    return prime_cache[(size_t)(n - (idx_t)1)];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety, though offset starts at 1 */

    /* first prime */
    unsigned long long p1 = nth_prime((idx_t)n);

    /* second prime: prime at index p1 */
    unsigned long long p2 = nth_prime((idx_t)p1);

    /* compute sum in seq_t type */
    seq_t result = (seq_t)p1;
    result += (seq_t)p2;
    return result;
}
