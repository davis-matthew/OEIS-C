/*
 * A145337  a(n) = d(p(n)+1) - d(p(n)-1), where d(m) = the number of divisors
 *          of m, p(n) = the n-th prime.
 * https://oeis.org/A145337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(p(n)+1) - d(p(n)-1), where p(n) is the n-th prime
 *            (0‑based), and d(m) is the number of positive divisors of m
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime generation – simple sieve, expanded lazily as needed.        */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list    = NULL;   /* array of primes, 0‑based */
static size_t   prime_count   = 0;       /* current number of primes stored */
static size_t   sieve_limit   = 0;       /* current upper bound of sieve */

/* Helper: estimate an upper bound for the (n+1)-th prime.
   The bound n*17 + 100 is more than enough for n <= 10 000
   (the 10 000th prime is 104 729).                          */
static size_t
estimate_upper_bound(size_t n)
{
    /* n is the index (0‑based), we need at least n+1 primes. */
    return (n + 1) * 17 + 100;
}

/* Perform a new sieve up to the given limit,
   replacing any existing prime list.                */
static void
sieve_up_to(size_t limit)
{
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Allocate a boolean array: 0 -> prime, 1 -> composite. */
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (is_comp == NULL) {
        /* Out of memory – abort. In practice this should not happen. */
        abort();
    }

    size_t   idx = 0;
    size_t   i;
    void     **ptr; /* unused; satisfy compiler */ (void)ptr;

    /* Sieve of Eratosthenes. */
    for (i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes up to limit. */
    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++idx;
        }
    }

    /* Allocate array with exact size. */
    prime_list = (uint64_t *)malloc(idx * sizeof(uint64_t));
    if (prime_list == NULL) {
        free(is_comp);
        abort();
    }

    /* Store primes. */
    size_t pos = 0;
    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[pos++] = (uint64_t)i;
        }
    }

    free(is_comp);
    prime_count = idx;
    sieve_limit = limit;
}

/* Ensure that at least (n+1) primes are available in prime_list. */
static void
ensure_primes(idx_t n)
{
    if (prime_count > (size_t)n) return;          /* already enough */

    size_t needed_limit = estimate_upper_bound(n);
    if (needed_limit <= sieve_limit) {
        /* Current sieve too small – grow. */
        needed_limit = sieve_limit * 2 + 100;
    }

    /* Sieve again with a larger limit. */
    sieve_up_to(needed_limit);
}

/* ------------------------------------------------------------------ */
/* Divisor counting – simple trial division using the prime list.     */
/* ------------------------------------------------------------------ */
static uint64_t
divisor_count(uint64_t m)
{
    if (m == 0) return 0;          /* undefined, return 0 */

    uint64_t result = 1;
    uint64_t temp   = m;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t p    = prime_list[i];
        uint64_t exp  = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp > 0) {
            result *= (exp + 1);
        }
    }

    if (temp > 1) {       /* remaining prime factor */
        result *= 2;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Lazily generate enough primes. */
    ensure_primes(n);

    /* Retrieve p(n). */
    uint64_t p = prime_list[(size_t)n];

    /* Compute d(p+1) and d(p-1). */
    uint64_t d_plus  = divisor_count(p + 1);
    uint64_t d_minus = divisor_count(p - 1);

    /* Difference, cast to seq_t. */
    int64_t diff = (int64_t)d_plus - (int64_t)d_minus;
    return (seq_t)diff;
}
