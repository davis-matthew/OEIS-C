/*
 * A121807  Partial sums of A004676.
 * https://oeis.org/A121807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = \sum_{k=1}^{n} \text{decimal_value_of_binary}(p_k) where
 *            p_k is the k-th prime
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 92, u64 n <= 8047,
 *            u128 n <= 10000, big any n
 * Helpers:   count_setbits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A004676 lists the binary representations of the primes:
   2, 3, 5, 7, 11, 13, 17, ...  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_setbits.h"

static seq_t *partial_sums = NULL;   /* sums[0] = 0, sums[i] = a(i) */
static size_t sum_cap = 0;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_sequence(void) {
    /* a safe upper bound to obtain at least 20000 primes   */
    const size_t limit = 200000;      /* > 10000th prime (104729)   */
    bool *is_prime = (bool*)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) primes_cnt++;

    /* store primes */
    primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = i;

    free(is_prime);

    /* compute partial sums */
    sum_cap = primes_cnt + 1;
    partial_sums = (seq_t*)malloc(sum_cap * sizeof(seq_t));
    partial_sums[0] = 0;

    for (size_t k = 1; k <= primes_cnt; ++k) {
        uint64_t p = primes[k-1];
        unsigned __int128 val = 0;
        int msb = 63;
        while (msb >= 0 && ((p >> msb) & 1ULL) == 0) msb--;
        for (int bit = msb; bit >= 0; --bit) {
            val = val * 10 + ((p >> bit) & 1ULL);
        }
        unsigned __int128 sum128 = (unsigned __int128)partial_sums[k-1] + val;
        partial_sums[k] = (seq_t)sum128;
    }
}

seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* extend if requested n exceeds current precomputed range */
    if ((size_t)n >= sum_cap) {
        /* grow sieve and recompute more primes if necessary */
        size_t current_limit = primes_cnt > 0 ? primes[primes_cnt-1] : 1;
        size_t new_limit = (size_t)n * 20 + 100;   /* rough upper bound */
        if (new_limit <= current_limit) new_limit = current_limit * 2 + 10;

        bool *is_prime = (bool*)malloc(new_limit + 1);
        for (size_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= new_limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= new_limit; j += i) is_prime[j] = false;
            }
        }

        size_t new_cnt = 0;
        for (size_t i = primes_cnt + 1; i <= new_limit; ++i)
            if (is_prime[i]) new_cnt++;

        uint64_t *new_primes = (uint64_t*)realloc(primes, (primes_cnt + new_cnt) * sizeof(uint64_t));
        size_t pos = primes_cnt;
        for (size_t i = primes_cnt + 1; i <= new_limit; ++i)
            if (is_prime[i]) new_primes[pos++] = i;
        primes = new_primes;
        size_t total_cnt = primes_cnt + new_cnt;

        seq_t *new_sums = (seq_t*)realloc(partial_sums, (total_cnt + 1) * sizeof(seq_t));
        for (size_t k = primes_cnt + 1; k <= total_cnt; ++k) {
            uint64_t p = primes[k-1];
            unsigned __int128 val = 0;
            int msb = 63;
            while (msb >= 0 && ((p >> msb) & 1ULL) == 0) msb--;
            for (int bit = msb; bit >= 0; --bit) {
                val = val * 10 + ((p >> bit) & 1ULL);
            }
            unsigned __int128 sum128 = (unsigned __int128)new_sums[k-1] + val;
            new_sums[k] = (seq_t)sum128;
        }
        primes_cnt = total_cnt;
        sum_cap = total_cnt + 1;
        partial_sums = new_sums;
        free(is_prime);
    }

    return partial_sums[n];
}
