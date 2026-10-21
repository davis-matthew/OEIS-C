/*
 * A094931  A measure of the excess of the mean of the set of 4 consecutive
 *          primes over the 2nd of the set.
 * https://oeis.org/A094931
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = max(0, prime(n-3)+prime(n-2)+prime(n-1)+prime(n) -
 *            4*prime(n-2))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) .. a(3) are undefined; for n<4 the function returns 0. */

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* static cache of primes */
static idx_t *prime_cache = NULL;
static size_t prime_cached = 0;

/* generate primes up to at least the nth prime (1‑based) */
static void ensure_primes(idx_t n)
{
    if (prime_cached >= n) return;

    /* upper bound for nth prime (Rosser's theorem) */
    double dn = (double)n;
    idx_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 3.0);

    /* sieve up to limit */
    size_t size = (size_t)limit + 1;
    unsigned char *is_prime = (unsigned char *)malloc(size);
    if (!is_prime) return;               /* allocation failure – abort silently */

    memset(is_prime, 1, size);
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* count primes needed */
    size_t needed = 0;
    for (idx_t i = 2; i <= limit && needed < n; ++i)
        if (is_prime[i]) ++needed;

    /* allocate and fill cache */
    idx_t *new_cache = (idx_t *)malloc(needed * sizeof(idx_t));
    if (!new_cache) { free(is_prime); return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit && pos < needed; ++i)
        if (is_prime[i]) new_cache[pos++] = i;

    free(is_prime);
    free(prime_cache);
    prime_cache = new_cache;
    prime_cached = needed;
}

/* sequence entry */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    ensure_primes(n);

    /* indices: prime(1)=prime_cache[0] */
    idx_t p0 = prime_cache[n - 4];   /* prime(n-3) */
    idx_t p1 = prime_cache[n - 3];   /* prime(n-2) */
    idx_t p2 = prime_cache[n - 2];   /* prime(n-1) */
    idx_t p3 = prime_cache[n - 1];   /* prime(n)   */

    /* use 64-bit intermediate to avoid overflow on small seq_t types */
    uint64_t sum = (uint64_t)p0 + (uint64_t)p1 + (uint64_t)p2 + (uint64_t)p3;
    uint64_t prod4 = (uint64_t)p1 * 4ULL;

    if (sum > prod4)
        return (seq_t)(sum - prod4);
    else
        return (seq_t)0;
}
