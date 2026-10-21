/*
 * A077336  Smallest squarefree number beginning with n.
 * https://oeis.org/A077336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the smallest square‑free integer whose decimal
 *            representation begins with the digits of n. The algorithm
 *            searches numbers with increasing number of trailing digits until
 *            it finds a square‑free one. It uses a simple sieve of primes up
 *            to 100000 and checks square‑freeness by trial division
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   to_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/to_uint64.h"

/* Array of powers of 10 (up to 10^18, safe for 64‑bit operands). */
static const uint64_t POW10[] = {
    1ULL,                  /**< 10⁰ */
    10ULL,                 /**< 10¹ */
    100ULL,                /**< 10² */
    1000ULL,               /**< 10³ */
    10000ULL,              /**< 10⁴ */
    100000ULL,             /**< 10⁵ */
    1000000ULL,            /**< 10⁶ */
    10000000ULL,           /**< 10⁷ */
    100000000ULL,          /**< 10⁸ */
    1000000000ULL,         /**< 10⁹ */
    10000000000ULL,        /**< 10¹⁰ */
    100000000000ULL,       /**< 10¹¹ */
    1000000000000ULL,      /**< 10¹² */
    10000000000000ULL,     /**< 10¹³ */
    100000000000000ULL,    /**< 10¹⁴ */
    1000000000000000ULL,   /**< 10¹⁵ */
    10000000000000000ULL,  /**< 10¹⁶ */
    100000000000000000ULL, /**< 10¹⁷ */
    1000000000000000000ULL/**< 10¹⁸ */
};
#define POW10_LEN (sizeof(POW10)/sizeof(POW10[0]))

/* Simple sieve of primes up to LIMIT.  The primes are stored in a static
   array; the function is called lazily on the first invocation of a(). */
#define PRIME_LIMIT 100000U
static uint32_t *SievePrimes = NULL;   /* dynamically allocated array of primes */
static size_t   PrimeCnt    = 0;        /* number of primes */

static void init_sieve(void)
{
    if (SievePrimes != NULL)         /* already initialised */
        return;

    /* Allocate enough space for the expected number of primes. */
    size_t max_primes = (size_t)(PRIME_LIMIT / 1); /* rough estimate */
    uint32_t *is_prime = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    if (!is_prime)
        return;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = 1;

    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes and allocate array. */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            ++PrimeCnt;

    SievePrimes = (uint32_t *)malloc(PrimeCnt * sizeof(uint32_t));
    if (!SievePrimes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            SievePrimes[idx++] = i;

    free(is_prime);
}

/* Return true iff the 64‑bit integer n is squarefree.  The function
   uses trial division by the pre‑computed primes.  It is safe to
   cast n to uint64_t because the algorithm limits the search to
   numbers that fit in 64 bits. */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n < 2)
        return true;   /* 0 and 1 are squarefree by convention */

    for (size_t i = 0; i < PrimeCnt; ++i) {
        uint32_t p = SievePrimes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > n)
            break;
        if (n % p2 == 0)
            return false;
    }
    /* If there remains a prime factor > sqrt(n), it cannot have
       multiplicity > 1 (otherwise its square would exceed n). */
    return true;
}

seq_t a(idx_t n)
{
    /* Initialise the prime sieve lazily. */
    init_sieve();

    /* Work with the operand n as a 64‑bit value.  For all values
       used in this program n fits into uint64_t. */
    uint64_t n0 = (uint64_t)n;

    /* Search numbers with increasing number of extra digits.  The
       outer loop runs over the amount t of trailing digits to append
       to the prefix n.  For each t we scan the full interval
       [n*10ᵗ, (n+1)*10ᵗ – 1] until a squarefree number is found. */
    for (int t = 0; t < (int)POW10_LEN; ++t) {
        /* Check for overflow of n0 * POW10[t]. */
        if (n0 > UINT64_MAX / POW10[t])
            break;                     /* cannot explore larger t */

        uint64_t low  = n0 * POW10[t];
        uint64_t high = low + POW10[t] - 1;   /* inclusive upper bound */

        for (uint64_t candidate = low; candidate <= high; ++candidate) {
            if (is_squarefree_uint64(candidate))
                return (seq_t)candidate;
        }
    }

    /* In practice this point is never reached for n ≥ 1 up to the
       tested limits, but return a defensive value if it ever happens. */
    return (seq_t)(-1);
}
