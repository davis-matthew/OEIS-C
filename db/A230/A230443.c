/*
 * A230443  Number of decompositions of 2n into a sum of two primes p2 >= p1
 *          such that the number of runs in binary expansion of p2-p1 is less
 *          than or equal to 4.
 * https://oeis.org/A230443
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decompositions 2n = p1+p2 with p2 ≥ p1 primes
 *            for which the number of runs in the binary representation of
 *            p2-p1 is ≤ 4. The function is computed by a simple sieve of
 *            Eratosthenes up to 2 n and a linear scan over the prime list
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_bit_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_bit_runs.h"

static idx_t prime_max_limit = 0;          /* highest number for which we have a sieve   */
static bool *prime_sieve = NULL;          /* prime_sieve[i] == true  ⇒ composite         */
static uint64_t *prime_list = NULL;       /* array of all primes ≤ prime_max_limit      */
static size_t prime_cnt = 0;              /* number of primes in prime_list             */

/* ------------------------------------------------------------------ */
/* Build (or extend) a sieve up to 'limit' and fill the prime list.   */
/* ------------------------------------------------------------------ */
static void build_prime_table(idx_t limit)
{
    if (limit <= prime_max_limit) {          /* already sufficient */
        return;
    }

    /* enlarge or create the sieve array */
    size_t new_cap = limit + 1;              /* indices 0 … limit inclusive */
    if (!prime_sieve) {
        prime_sieve = (bool*)calloc(new_cap, sizeof(bool));
    } else {
        size_t old_cap = prime_max_limit + 1;
        prime_sieve = (bool*)realloc(prime_sieve, new_cap * sizeof(bool));
        memset(prime_sieve + old_cap, 0, (new_cap - old_cap) * sizeof(bool));
    }

    /* Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!prime_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                prime_sieve[j] = true;
            }
        }
    }

    /* Collect primes into list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!prime_sieve[i]) ++cnt;
    }

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!prime_sieve[i]) new_list[pos++] = i;
    }

    free(prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_max_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Count runs of 1/0 in binary representation of x.                   */

/* ------------------------------------------------------------------ */
/* Main sequence function: a(n)                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;    /* undefined below offset */

    idx_t target = 2 * n;          /* even number to decompose */

    /* Ensure we have primes up to 'target'. */
    build_prime_table(target);

    uint64_t count = 0;

    /* Scan primes p1 ≤ target/2 (ensures p2 ≥ p1). */
    idx_t half = target / 2;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= half; ++i) {
        uint64_t p1 = prime_list[i];
        uint64_t p2 = target - p1;

        /* p2 is automatically prime by the sieve, because target ≤ prime_max_limit. */
        if (!prime_sieve[p2]) {
            uint64_t diff = p2 - p1;
            if (count_bit_runs(diff) <= 4) {
                ++count;
            }
        }
    }

#ifdef __cplusplus
    return static_cast<seq_t>(count);
#else
    return (seq_t)count;
#endif
}
