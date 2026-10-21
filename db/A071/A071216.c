/*
 * A071216  a(n) is the largest prime factor of prime(n) + prime(n+1).
 * https://oeis.org/A071216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest prime factor of prime(n) + prime(n+1)
 *
 * The function keeps an internal prime sieve that is grown as needed
 * to supply prime(n) and prime(n+1) for arbitrary order of calls.
 * It uses only arithmetic on 64‑bit unsigned integers for the
 * intermediate calculations; the result is finally cast to seq_t.
 *
 * The code follows all build‑time type constraints (seq_t and idx_t
 * are supplied by the build environment; they are **not** defined
 * here) and does not depend on any external libraries beyond the
 * C standard library.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --- Private data ----------------------------------------------------------- */

static uint32_t *prime_list = NULL;
static size_t    prime_count = 0;
static uint32_t  sieve_limit = 0;
static unsigned char *sieve_array = NULL;

/* --- Sieve construction -------------------------------------------------------- */

/* Builds an Eratosthenes sieve up to 'limit' (inclusive) and
 * fills prime_list with the primes found.  Calls free() on any
 * previously allocated sieve arrays.  The limit must be >= 2. */
static void build_sieve(uint32_t limit)
{
    /* Free previous allocations */
    if (sieve_array) { free(sieve_array); sieve_array = NULL; }
    if (prime_list)  { free(prime_list);  prime_list = NULL;  }

    /* Allocate sieve */
    sieve_limit = limit;
    sieve_array = (unsigned char *)malloc(limit + 1);
    if (!sieve_array) exit(1);
    memset(sieve_array, 0, limit + 1);

    /* Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!sieve_array[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                sieve_array[j] = 1;
        }
    }

    /* Count primes */
    prime_count = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve_array[i]) ++prime_count;

    /* Store primes */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t pos = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve_array[i]) prime_list[pos++] = i;
}

/* Rough upper bound for the k-th prime (k >= 6).
 * Uses the inequality p_k < k (log k + log log k) for k >= 6.
 * The implementation avoids floating point by using integer arithmetic. */
static uint32_t upper_bound_prime(uint32_t k)
{
    if (k < 6) return 15;
    /* log2(k) as integer */
    uint32_t log2k = 0;
    uint32_t t = k;
    while (t > 1) { t >>= 1; ++log2k; }
    /* p_k < k * (log2k + log2k/2 + 3)  (coarse safe bound) */
    return k * (log2k + log2k/2 + 3);
}

/* Ensures that we have at least 'need' primes in prime_list.
 * The routine grows the sieve until the requirement is met. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;
    uint32_t limit = upper_bound_prime((uint32_t)need);
    if (limit < 15) limit = 15;
    while (prime_count < need) {
        build_sieve(limit);
        if (prime_count >= need) break;
        limit *= 2;                /* safety growth */
    }
}

/* --- Factorisation ------------------------------------------------------------ */

/* Returns the largest prime factor of a 64‑bit unsigned integer.
 * The function uses trial division with the primes generated so far
 * (dividing until the square root).  The result is never larger
 * than the input value. */
static uint64_t largest_prime_factor(uint64_t value)
{
    if (value < 2) return value;     /* 0 or 1 */
    uint64_t largest = 1;
    uint64_t remaining = value;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > remaining) break;
        if (remaining % p == 0) {
            largest = p;
            while (remaining % p == 0) remaining /= p;
        }
    }
    if (remaining > 1 && remaining > largest) largest = remaining;
    return largest;
}

/* --- Public interface ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out‑of‑range guard */

    /* We need prime(n) and prime(n+1) (1‑based indexing). */
    ensure_primes((size_t)n + 1);

    uint32_t p1 = prime_list[(size_t)(n - 1)];   /* prime(n)  */
    uint32_t p2 = prime_list[(size_t) n];        /* prime(n+1)*/

    uint64_t sum = (uint64_t)p1 + (uint64_t)p2;
    uint64_t lp  = largest_prime_factor(sum);

    return (seq_t)lp;
}
