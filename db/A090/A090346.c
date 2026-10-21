/*
 * A090346  Number of divisors of prime(n) + prime(n+1).
 * https://oeis.org/A090346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(prime(n) + prime(n+1)) (number of divisors)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine generates a small prime sieve on first use and counts
   divisors by trial division with the same primes.  It works for all
   n ≥ 1 and is fast enough for n up to at least 10000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static size_t prime_limit   = 0;
static size_t prime_cap     = 0;
static int    *prime_list   = NULL;
static size_t prime_count   = 0;
static bool    primes_ready = false;

/* ------------------------------------------------------------------ */
/* Simple sieve of Eratosthenes that stores primes in prime_list.     */
/* ------------------------------------------------------------------ */
static void build_primes(size_t limit)
{
    if (limit < 2) return;
    bool *is_composite = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= limit; ++i) is_composite[i] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (!is_composite[p])
            for (size_t x = p * p; x <= limit; x += p)
                is_composite[x] = true;

    /* Determine required capacity. */
    size_t estimate = limit / 10 + 10;   /* crude estimate for #primes */
    if (prime_cap < estimate) {
        int *tmp = (int*)realloc(prime_list, estimate * sizeof(int));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = estimate;
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) {
            if (cnt == prime_cap) {     /* grow if not enough space */
                prime_cap *= 2;
                int *tmp = (int*)realloc(prime_list, prime_cap * sizeof(int));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[cnt++] = (int)i;
        }

    prime_count = cnt;
    prime_limit = limit;
    primes_ready = true;
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/* Ensure that at least 'need' primes are available.  Increase limit until
   the count is reached.                                                     */
static void ensure_primes(idx_t need)
{
    if (!primes_ready) {
        /* Start with a small limit that will be expanded if needed. */
        size_t start = 1000;
        build_primes(start);
    }
    while (prime_count < (size_t)need) {
        /* Double the search space each iteration.  It will overshoot,
           but keeps the number of rebuilds logarithmic. */
        size_t new_limit = prime_limit * 2 + 100;
        build_primes(new_limit);
    }
}

/* ------------------------------------------------------------------ */
/* Return the k-th prime (1‑based).                                       */
static inline int get_prime(idx_t k)
{
    ensure_primes(k);
    return prime_list[k - 1];
}

/* ------------------------------------------------------------------ */
/* Count divisors of a 64‑bit integer using the prime list.              */
static uint64_t divisor_count_uint64(uint64_t x)
{
    uint64_t result = 1;
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (tmp > 1) result *= 2;   /* remaining prime factor > sqrt(original) */
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                               */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but not required */
    int p1 = get_prime(n);
    int p2 = get_prime(n + 1);
    uint64_t sum = (uint64_t)p1 + (uint64_t)p2;
    return (seq_t)divisor_count_uint64(sum);
}
