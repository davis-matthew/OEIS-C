/*
 * A047948  Smallest of three consecutive primes with a difference of 6:
 *          primes p such that p+6 and p+12 are the next two primes.
 * https://oeis.org/A047948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A047948:
   a(n) = the smallest prime p such that p+6 and p+12 are also prime.
   The function searches for consecutive prime triples with equal gaps of 6
   and returns the first element of each triple.  No explicit formula exists;
   a sieve is used to generate primes on demand.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static uint64_t sieve_limit = 0;
static uint8_t *sieve_bits = NULL;
static uint64_t *primes = NULL;
static size_t primes_len = 0;

static seq_t *triplets = NULL;
static size_t triplets_len = 0;
static size_t triplets_cap = 0;
static size_t prime_processed_idx = 0;

/* bit helpers – 1 means composite for odd numbers only */
static inline int get_bit(uint64_t n) {
    uint64_t idx = n >> 1;
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1;
}
static inline void set_bit(uint64_t n) {
    uint64_t idx = n >> 1;
    sieve_bits[idx >> 3] |= (1 << (idx & 7));
}

/* build sieve up to 'limit' inclusively; rebuilds from scratch */
static void build_sieve(uint64_t limit) {
    if (limit == sieve_limit)
        return;

    free(sieve_bits);
    free(primes);
    sieve_bits = NULL;
    primes = NULL;
    primes_len = 0;

    /* sizes for odd numbers only */
    size_t odd_cnt = (limit >> 1) + 1;           /* number of odd indices <= limit */
    size_t byte_cnt = (odd_cnt + 7) >> 3;
    sieve_bits = (uint8_t*)calloc(byte_cnt, 1);
    if (!sieve_bits)
        abort();

    /* Eratosthenes on only odd numbers */
    for (uint64_t i = 3; i * i <= limit; i += 2) {
        if (get_bit(i))
            continue;          /* composite */
        for (uint64_t j = i * i; j <= limit; j += i << 1)
            set_bit(j);
    }

    /* gather primes */
    size_t est_cap = limit / 10 + 10;
    primes = (uint64_t*)malloc(est_cap * sizeof(uint64_t));
    if (!primes)
        abort();
    size_t cap = est_cap;

    /* prime 2 */
    primes[primes_len++] = 2;
    for (uint64_t n = 3; n <= limit; n += 2) {
        if (!get_bit(n)) {
            if (primes_len == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_len++] = n;
        }
    }
    sieve_limit = limit;
}

/* ensure that at least 'need' triples have been found */
static void ensure_triplets(size_t need) {
    if (triplets_len >= need)
        return;

    while (true) {
        /* if not enough primes yet, enlarge sieve */
        while (prime_processed_idx + 2 >= primes_len) {
            uint64_t new_limit = (sieve_limit == 0) ? 1000000ULL
                                                    : sieve_limit * 2ULL;
            build_sieve(new_limit);
        }

        /* scan for triples starting from last processed prime */
        for (size_t i = prime_processed_idx; i + 2 < primes_len; ++i) {
            uint64_t p1 = primes[i];
            uint64_t p2 = primes[i + 1];
            uint64_t p3 = primes[i + 2];
            if ((p2 - p1 == 6) && (p3 - p2 == 6)) {
                /* store p1 */
                if (triplets_len == triplets_cap) {
                    size_t new_cap = triplets_cap ? triplets_cap << 1 : 16;
                    seq_t *new_arr;
#ifdef __cplusplus
                    /* C++: allocate constructed objects */
                    new_arr = new seq_t[new_cap];
                    if (triplets) {
                        for (size_t j = 0; j < triplets_len; ++j)
                            new_arr[j] = triplets[j];
                        delete[] triplets;
                    }
#else
                    /* C: raw allocation, no construction needed */
                    new_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
                    if (triplets) {
                        memcpy(new_arr, triplets, triplets_len * sizeof(seq_t));
                        free(triplets);
                    }
#endif
                    triplets = new_arr;
                    triplets_cap = new_cap;
                }
                triplets[triplets_len++] = (seq_t)p1;
                if (triplets_len >= need) {
                    prime_processed_idx = i + 1;
                    return;
                }
            }
        }
        /* all current primes exhausted; step forward for next search */
        prime_processed_idx = primes_len - 2;
        /* next loop will enlarge sieve if still needed */
    }
}

/* public interface */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined below offset */
    size_t idx = (size_t)n;
    ensure_triplets(idx);
    return triplets[idx - 1];
}
