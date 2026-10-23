/*
 * A049881  a(n) is the number of distinct sums of 3 different primes chosen
 *          from the first n primes.
 * https://oeis.org/A049881
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   popcount_bitset, shift_and_or
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of distinct sums of 3 different primes taken from the
 * first n primes. */
/* a(n) = |{p_i + p_j + p_k : 0 ≤ i < j < k < n}|   (n ≥ 3) */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/popcount_bitset.h"
#include "../utils/shift_and_or.h"

/* Return the first n primes, allocating an array of length n.  The caller
 * must free the returned pointer. */
static uint64_t *get_first_n_primes(idx_t n)
{
    if (n == 0) return NULL;

    /* Rough upper bound for the n-th prime:  n(ln n + ln ln n)  for n≥6,
     * a small fixed bound for n<6.  Add a small safety margin. */
    double nn = (double)n;
    double estimate;
    if (n < 6) {
        estimate = 15.0;
    } else {
        estimate = nn * (log(nn) + log(log(nn))) + 3.0;
    }
    idx_t limit = (idx_t)ceil(estimate) + 1;

    uint8_t *is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_comp) return NULL;

    uint64_t *primes = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!primes) {
        free(is_comp);
        return NULL;
    }

    size_t count = 0;
    for (idx_t i = 2; i <= limit && count < n; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[count++] = (uint64_t)i;
            if ((idx_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);

    /* In the very unlikely event the estimate was too low, extend the sieve. */
    while (count < n) {
        /* increase limit by 10% and repeat */
        limit = (idx_t)(limit * 1.1) + 1;
        is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
        if (!is_comp) { free(primes); return NULL; }

        count = 0;
        for (idx_t i = 2; i <= limit && count < n; ++i) {
            if (!is_comp[(size_t)i]) {
                primes[count++] = (uint64_t)i;
                if ((idx_t)i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[(size_t)j] = 1;
                }
            }
        }
        free(is_comp);
    }

    return primes;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;      /* offset 3; undefined for n<3 */

    /* Get first n primes. */
    uint64_t *primes = get_first_n_primes(n);
    if (!primes) return 0;

    /* Max sums needed. */
    uint64_t max_pair_sum = primes[n - 1] + primes[n - 2];
    uint64_t max_triple_sum = primes[n - 1] + primes[n - 2] + primes[n - 3];

    size_t pair_words = (size_t)((max_pair_sum >> 6) + 1);
    size_t triple_words = (size_t)((max_triple_sum >> 6) + 1);

    uint64_t *pair_bits   = (uint64_t *)calloc(pair_words, sizeof(uint64_t));
    uint64_t *triple_bits = (uint64_t *)calloc(triple_words, sizeof(uint64_t));
    if (!pair_bits || !triple_bits) {
        free(primes);
        free(pair_bits);
        free(triple_bits);
        return 0;
    }

    /* Main loop: for each prime index k. */
    for (idx_t k = 0; k < n; ++k) {
        if (k >= 2) {
            /* Add all triples using prime[k] + each existing pair sum. */
            shift_and_or(pair_bits, pair_words, triple_bits, triple_words, primes[k]);
        }
        /* Add new pair sums (i,k) for all i<k. */
        for (idx_t i = 0; i < k; ++i) {
            uint64_t sum = primes[i] + primes[k];
            size_t word = (size_t)(sum >> 6);
            uint64_t mask = 1ULL << (sum & 63);
            pair_bits[word] |= mask;
        }
    }

    seq_t result = popcount_bitset(triple_bits, triple_words);

    free(primes);
    free(pair_bits);
    free(triple_bits);
    return result;
}
