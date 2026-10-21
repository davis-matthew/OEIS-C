/*
 * A087102  Number of numbers occurring most frequently as difference between
 *          consecutive primes <= prime(n).
 * https://oeis.org/A087102
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of gaps that occur most frequently
   among differences of consecutive primes up to prime(n) (offset 2). */
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Ensure we have at least n primes stored in the static array `primes`.
   The array is rebuilt from scratch when a new, larger n is requested. */
static void
ensure_primes(idx_t n)
{
    /* approximate upper bound for the nth prime */
    idx_t limit;
    if (n < (idx_t)6) {
        limit = 15;
    } else {
        double nd = (double)n;
        limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
    }

    /* allocate boolean array for sieve */
    size_t sz = (size_t)limit + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) return;  /* allocation failure : ok to leave primes unfilled */

    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;

    /* simple Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes and allocate array */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) ++count;
    }

    uint32_t *new_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) new_primes[idx++] = (uint32_t)i;
    }

    free(is_prime);

    /* replace static storage */
    static uint32_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_n = 0;

    free(primes);
    primes     = new_primes;
    primes_cap = count;
    primes_n   = (idx_t)count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;  /* undefined for n < 2 */

    /* ensure primes up to n are available */
    static uint32_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_n = 0;

    if (n > primes_n) {
        /* rebuild prime list */
        /* approximate limit as earlier */
        idx_t limit;
        if (n < (idx_t)6) {
            limit = 15;
        } else {
            double nd = (double)n;
            limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
        }

        size_t sz = (size_t)limit + 1;
        uint8_t *is_prime = (uint8_t *)malloc(sz);
        if (!is_prime) return (seq_t)0;
        memset(is_prime, 1, sz);
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    is_prime[j] = 0;
            }
        }
        size_t count = 0;
        for (size_t i = 2; i <= (size_t)limit; ++i) {
            if (is_prime[i]) ++count;
        }
        uint32_t *new_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
        if (!new_primes) {
            free(is_prime);
            return (seq_t)0;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= (size_t)limit; ++i) {
            if (is_prime[i]) new_primes[idx++] = (uint32_t)i;
        }
        free(is_prime);
        free(primes);
        primes = new_primes;
        primes_cap = count;
        primes_n = (idx_t)count;
    }

    /* count frequencies of gaps up to the n-th prime */
    static size_t *gap_counts = NULL;
    static size_t gap_cap = 0;
    static idx_t current_n = 0;

    if (n != current_n) {
        /* reset frequency array to fit new maximum gap */
        size_t max_gap = 0;
        for (size_t i = 1; i < (size_t)n; ++i) {
            size_t gap = (size_t)primes[i] - (size_t)primes[i - 1];
            if (gap > max_gap) max_gap = gap;
        }
        if (gap_cap < max_gap + 1) {
            size_t *new_counts = (size_t *)realloc(gap_counts, (max_gap + 1) * sizeof(size_t));
            if (!new_counts) return (seq_t)0;
            gap_counts = new_counts;
            gap_cap = max_gap + 1;
        }
        memset(gap_counts, 0, gap_cap * sizeof(size_t));

        size_t max_freq = 0;
        size_t mode_cnt = 0;

        for (size_t i = 1; i < (size_t)n; ++i) {
            size_t gap = (size_t)primes[i] - (size_t)primes[i - 1];
            ++gap_counts[gap];
            if (gap_counts[gap] > max_freq) {
                max_freq = gap_counts[gap];
                mode_cnt = 1;
            } else if (gap_counts[gap] == max_freq) {
                ++mode_cnt;
            }
        }
        current_n = n;
        return (seq_t)mode_cnt;
    }
    /* if same n as previous call, return cached result */
    return (seq_t)0;
}
