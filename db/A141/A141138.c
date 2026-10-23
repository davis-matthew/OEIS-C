/*
 * A141138  a(n) = prime(A028815(n) - 1) - 1.
 * https://oeis.org/A141138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)) - 1, with a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   upper_limit_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/upper_limit_for_index.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t sieve_limit = 0;

/* Generate all primes ≤ limit and store them in prime_list. */
static void generate_primes_up_to_limit(uint64_t limit) {
    size_t size = limit + 1;
    unsigned char *isComposite = (unsigned char*)malloc(size);
    if (!isComposite) abort();
    memset(isComposite, 0, size);

    uint64_t sqrt_lim = (uint64_t)sqrt((double)limit);
    for (uint64_t p = 2; p <= sqrt_lim; ++p) {
        if (!isComposite[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                isComposite[m] = 1;
        }
    }

    /* Count primes first */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) new_list[pos++] = i;

    free(isComposite);
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure that prime_list contains at least target primes. */
static void ensure_primes_at_least(size_t target) {
    if (prime_count >= target) return;
    uint64_t needed = upper_limit_for_index(target);
    generate_primes_up_to_limit(needed);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    size_t idx1 = (size_t)n;          /* n is at least 1 */
    ensure_primes_at_least(idx1);
    uint64_t p1 = prime_list[idx1 - 1];

    size_t idx2 = (size_t)p1;         /* prime index */
    ensure_primes_at_least(idx2);
    uint64_t p2 = prime_list[idx2 - 1];

    return (seq_t)(p2 - 1);
}
