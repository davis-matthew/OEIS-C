/*
 * A173586  Decimal values a(n) of the binary numbers b(n) obtained by
 *          starting from first prime number (2), sequentially concatenating
 *          the binary representation of all prime numbers till n-th prime,
 *          and after that, sequentially concatenating the binary
 *          representation of all prime numbers, from (n-1)th till...
 * https://oeis.org/A173586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = binary_to_decimal(concatenate( binary of first n primes
 *            in order, then binary of primes n−1 … 2, 1 )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the bit length of a 64‑bit unsigned integer (0 → 0). */
static unsigned bitlen_uint64(uint64_t x)
{
    unsigned b = 0;
    while (x) {
        ++b;
        x >>= 1;
    }
    return b;
}

/* Cached prime list. */
static uint32_t *prime_cache = NULL;   /* array of first primes */
static size_t   prime_count = 0;       /* number of primes stored     */
static size_t   prime_cap   = 0;       /* allocation capacity        */

/* Ensure that the first `n` primes are stored in prime_cache. */
static void ensure_primes(size_t n)
{
    if (prime_count >= n)
        return;

    /* Grow cache if necessary. */
    if (prime_cap < n) {
        size_t new_cap = prime_cap ? (prime_cap << 1) : 16;
        while (new_cap < n) new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
        if (!tmp) return;   /* OOM – caller must handle failure */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    /* Initialise the cache if empty. */
    if (prime_count == 0) {
        prime_cache[0] = 2;
        prime_count = 1;
    }

    uint32_t candidate = prime_cache[prime_count - 1] + 1;
    while (prime_count < n) {
        int is_prime = 1;
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = prime_cache[j];
            if ((uint64_t)p * p > (uint64_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            prime_cache[prime_count++] = candidate;
        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* below the offset – undefined */

    size_t N = (size_t)n;              /* size_t for loops, safe up to 10k */
    ensure_primes(N);

    seq_t res = 0;

    /* Forward part: primes[0] … primes[N-1] */
    for (size_t i = 0; i < N; ++i) {
        seq_t p = (seq_t)prime_cache[i];
        unsigned bl = bitlen_uint64((uint64_t)p);
        res = (res << bl) | p;
    }

    /* Backward part: primes[N-2] … primes[0] (if N ≥ 2) */
    if (N >= 2) {
        for (size_t i = N - 2; ; --i) {
            seq_t p = (seq_t)prime_cache[i];
            unsigned bl = bitlen_uint64((uint64_t)p);
            res = (res << bl) | p;
            if (i == 0) break;          /* avoid unsigned underflow */
        }
    }

    return res;
}
