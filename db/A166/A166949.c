/*
 * A166949  The count of largest prime factors in n-th composite.
 * https://oeis.org/A166949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A071178(A002808(n))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = exponent of the largest prime factor of the n‑th composite number */

#include <stdbool.h>
#include <stdint.h>

/* --------- sieve of Eratosthenes (fixed limit) --------------------------------- */
#define LIMIT_PRIME 200000
#define MAX_PRIMES  20000

static uint32_t primes[MAX_PRIMES];
static idx_t    prime_count = 0;
static bool     primes_ready = false;

static void fill_primes(void)
{
    static bool is_prime[LIMIT_PRIME + 1];
    for (idx_t i = 0; i <= LIMIT_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; (uint64_t)i * i <= LIMIT_PRIME; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT_PRIME; j += i)
                is_prime[j] = false;

    for (idx_t i = 2; i <= LIMIT_PRIME; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = (uint32_t)i;
            if (prime_count == MAX_PRIMES) break;
        }
    }
    primes_ready = true;
}

/* --------- test for compositeness ------------------------------------------- */
static inline bool is_composite(idx_t n)
{
    if (n < 4) return false;
    for (idx_t i = 0; i < prime_count &&
            (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return true;
    }
    return false;
}

/* --------- exponent of the largest prime factor ------------------------------ */
static seq_t largest_prime_factor_exponent(uint64_t n)
{
    seq_t expLargest = 0;
    uint64_t largest = 0;
    uint64_t temp    = n;

    for (idx_t i = 0; i < prime_count &&
            (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (p > largest) {
                largest   = p;
                expLargest = exp;
            }
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        if (temp > largest) {
            largest   = temp;
            expLargest = 1;
        }
    }
    return expLargest;
}

/* --------- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* N/A outside offset */

    if (!primes_ready) fill_primes();

    idx_t count = 0;
    idx_t candidate = 4;           /* first composite */

    while (true) {
        if (is_composite(candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }

    return largest_prime_factor_exponent((uint64_t)candidate);
}
