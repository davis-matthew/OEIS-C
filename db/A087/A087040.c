/*
 * A087040  2nd largest prime factor of n-th composite number.
 * https://oeis.org/A087040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second largest prime factor of the n‑th composite number
 *            * (counting multiplicity; if the largest prime factor occurs at
 *            least twice, * then the second largest is that same prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const size_t PRIME_LIMIT = 200000;          /* enough for n up to ~10⁵ */
static uint64_t *seq_a087040_primes = NULL;
static size_t seq_a087040_prime_cnt = 0;
static bool seq_a087040_primes_inited = false;

/* Build the list of primes up to PRIME_LIMIT once. */
static void seq_a087040_init_primes(void)
{
    if (seq_a087040_primes_inited) return;

    bool *sieve = (bool *)malloc(PRIME_LIMIT + 1);
    if (!sieve) return;
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes to allocate exact storage */
    size_t pcnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++pcnt;

    seq_a087040_primes = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    if (!seq_a087040_primes) {
        free(sieve);
        return;
    }
    size_t k = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) seq_a087040_primes[k++] = (uint64_t)i;

    seq_a087040_prime_cnt = pcnt;
    free(sieve);
    seq_a087040_primes_inited = true;
}

/* Test whether x is prime using the pregenerated prime table. */
static bool seq_a087040_is_prime(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < seq_a087040_prime_cnt; ++i) {
        uint64_t p = seq_a087040_primes[i];
        if (p * p > x) break;
        if (x % p == 0) return x == p;
    }
    return true;
}

/* Return the second largest prime factor of num, counting multiplicity. */
static seq_t seq_a087040_second_largest_factor(uint64_t num)
{
    uint64_t max1 = 0, max2 = 0;
    uint64_t max1cnt = 0;
    uint64_t tmp = num;

    for (size_t i = 0;
         i < seq_a087040_prime_cnt &&
         seq_a087040_primes[i] * seq_a087040_primes[i] <= tmp;
         ++i) {
        uint64_t p = seq_a087040_primes[i];
        if (tmp % p == 0) {
            uint64_t count = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++count;
            }
            if (p > max1) {
                max2 = max1;
                max1 = p;
                max1cnt = count;
            } else if (p == max1) {
                max1cnt += count;
            } else if (p > max2) {
                max2 = p;
            }
        }
    }

    if (tmp > 1) {                     /* remaining prime factor */
        uint64_t p = tmp;
        uint64_t count = 1;
        if (p > max1) {
            max2 = max1;
            max1 = p;
            max1cnt = count;
        } else if (p == max1) {
            max1cnt += count;
        } else if (p > max2) {
            max2 = p;
        }
    }

    if (max1cnt >= 2) return (seq_t)max1;
    if (max2 == 0)   return (seq_t)max1;
    return (seq_t)max2;
}

/* External linkage: the required entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 per OEIS offset */

    seq_a087040_init_primes();

    /* Cache discovered composites. */
    static uint64_t *comps       = NULL;
    static size_t  comps_cnt    = 0;
    static uint64_t next_candidate = 4;   /* first composite number */

    /* Grow the composite list until it contains at least n composites. */
    while (comps_cnt < (size_t)n) {
        while (seq_a087040_is_prime(next_candidate)) ++next_candidate;
        uint64_t *tmp_ptr = (uint64_t *)realloc(comps,
                                                (comps_cnt + 1) * sizeof(uint64_t));
        if (!tmp_ptr) return (seq_t)0;   /* allocation failure – unlikely in tests */
        comps = tmp_ptr;
        comps[comps_cnt++] = next_candidate;
        ++next_candidate;
    }

    uint64_t comp_num = comps[(size_t)(n - 1)];
    return seq_a087040_second_largest_factor(comp_num);
}
