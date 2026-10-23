/*
 * A164275  The absolute value of (the n-th isolated composite minus the n-th
 *          isolated prime).
 * https://oeis.org/A164275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |A014574(n) - A007510(n)|
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of the required function */
seq_t a(idx_t n);

/* static storage for the generated sequences */
static uint32_t *iso_comps = NULL;
static size_t iso_comps_cnt = 0;
static size_t iso_comps_cap = 0;

static uint32_t *iso_primes = NULL;
static size_t iso_primes_cnt = 0;
static size_t iso_primes_cap = 0;

static bool initialized = false;

/* helper routine that builds both sequences up to a fixed upper bound */
static void build_sequences(void)
{
    const idx_t LIMIT = 2000000;  /* enough to cover n up to 10 000 */

    /* sieve of Eratosthenes – use byte array instead of bitset for simplicity */
    unsigned char *isPrime = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!isPrime)
        return;  /* out of memory – caller will handle the zero result */

    for (idx_t i = 0; i <= LIMIT; ++i)
        isPrime[i] = 1;
    isPrime[0] = 0;
    isPrime[1] = 0;

    for (idx_t i = 2; (idx_t)(i * i) <= LIMIT; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                isPrime[j] = 0;
        }
    }

    /* allocate initial storage for the two sequences */
    iso_comps_cap = 15000;
    iso_comps = (uint32_t *)malloc(iso_comps_cap * sizeof(uint32_t));

    iso_primes_cap = 140000;
    iso_primes = (uint32_t *)malloc(iso_primes_cap * sizeof(uint32_t));

    iso_comps_cnt = 0;
    iso_primes_cnt = 0;

    /* isolated composites – averages of twin prime pairs */
    for (idx_t p = 3; p + 2 <= LIMIT; p += 2) {
        if (isPrime[p] && isPrime[p + 2]) {
            if (iso_comps_cnt == iso_comps_cap) {
                iso_comps_cap *= 2;
                iso_comps = (uint32_t *)realloc(iso_comps, iso_comps_cap * sizeof(uint32_t));
            }
            iso_comps[iso_comps_cnt++] = (uint32_t)(p + 1);
        }
    }

    /* isolated primes – primes that are NOT part of a twin pair */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!isPrime[p])
            continue;

        bool left_twin  = (p >= 3) && isPrime[p - 2];
        bool right_twin = (p + 2 <= LIMIT) && isPrime[p + 2];
        if (!left_twin && !right_twin) {
            if (iso_primes_cnt == iso_primes_cap) {
                iso_primes_cap *= 2;
                iso_primes = (uint32_t *)realloc(iso_primes, iso_primes_cap * sizeof(uint32_t));
            }
            iso_primes[iso_primes_cnt++] = (uint32_t)p;
        }
    }

    free(isPrime);
    initialized = true;
}

/* external entry point ---------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool once = false;
    if (!once) {
        build_sequences();
        once = true;
    }

    if (n == 0 || n > iso_comps_cnt || n > iso_primes_cnt)
        return (seq_t)0; /* undefined for n outside range, but caller can ignore */

    idx_t idx = n - 1;
    long long comp  = (long long)iso_comps[idx];
    long long prime = (long long)iso_primes[idx];
    long long diff  = comp - prime;
    if (diff < 0)
        diff = -diff;
    return (seq_t)diff;
}
