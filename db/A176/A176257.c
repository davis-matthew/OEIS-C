/*
 * A176257  Numbers of the form 4k-1 with greatest prime divisor of the form
 *          4m+1.
 * https://oeis.org/A176257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer of the form 4k-1 whose greatest prime
 *            divisor is 4m+1
 * Range:     u8 n <= 18, u16 n <= 6733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- prime sieve ------------------------------------------ */
#define PRIME_LIMIT 2000000          /* sufficient for n up to about 10000 */

static bool primes_initialized = false;
static size_t primes_capacity = 0;
static uint32_t *primes = NULL;      /* prime list up to PRIME_LIMIT */
static size_t primes_count = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    size_t limit = PRIME_LIMIT;
    primes_capacity = 200000;                      /* initial estimate */
    primes = (uint32_t *)malloc(primes_capacity * sizeof(uint32_t));
    if (!primes)
        return;   /* allocation failure – subsequent calls will just fail */

    bool *sieve = (bool *)calloc(limit + 1, 1);
    if (!sieve) {
        free(primes);
        return;
    }

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (primes_count >= primes_capacity) {
                primes_capacity <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_capacity * sizeof(uint32_t));
                if (!tmp) { free(sieve); free(primes); return; }
                primes = tmp;
            }
            primes[primes_count++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    sieve[(size_t)j] = 1;
            }
        }
    }

    free(sieve);
    primes_initialized = true;
}

/* ---------- greatest prime divisor of a uint64_t ------------------- */
static uint64_t greatest_prime_divisor_uint64(uint64_t n)
{
    uint64_t temp = n;
    uint64_t gpf = 0;

    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            gpf = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)   /* remaining factor is prime */
        gpf = temp;

    return gpf;
}

/* ------------------- main sequence function ------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for offset 0, sequence starts at 1 */

    /* Ensure that the prime list is available */
    if (!primes_initialized)
        init_primes();

    /* Find the n-th qualifying number */
    size_t found = 0;
    uint64_t candidate = 3;   /* start with the first 4k-1 candidate */

    while (found < n) {
        /* advance to the next 4k-1 */
        candidate += 4;

        /* compute greatest prime factor */
        uint64_t gpf = greatest_prime_divisor_uint64(candidate);

        /* skip if candidate is prime itself */
        if (gpf == candidate)
            continue;

        /* check the 4m+1 condition */
        if ((gpf & 3ULL) == 1ULL)   /* gpf % 4 == 1 */
            ++found;
    }

    return (seq_t)candidate;
}
