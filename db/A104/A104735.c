/*
 * A104735  Numbers n such that n and digital sum of n-th prime are primes.
 * https://oeis.org/A104735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer i such that i is prime and the digital
 *            sum of the i‑th prime is also prime
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_sum_uint64, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Uses "is_prime_uint64" and "digit_sum_uint64" utilities. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/digit_sum_uint64.h"

/* Dynamic array of prime values: prime_list[k] holds the (k+1)‑th prime. */
static seq_t *prime_list = NULL;
static idx_t  prime_cnt  = 0;
static size_t prime_cap  = 0;

/* Dynamic array of indices that satisfy the condition. */
static idx_t *qual_list = NULL;
static idx_t  qual_cnt  = 0;
static size_t qual_cap  = 0;

/* Next integer to test. */
static idx_t next_test_index = 2;

/* Ensure prime_list can hold at least 'need' elements. */
static void ensure_prime_cap(idx_t need)
{
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        while (new_cap < (size_t)need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Ensure qual_list can hold at least 'need' elements. */
static void ensure_qual_cap(idx_t need)
{
    if (qual_cap < (size_t)need) {
        size_t new_cap = qual_cap ? qual_cap << 1 : 16;
        while (new_cap < (size_t)need)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(qual_list, new_cap * sizeof(idx_t));
        qual_list = tmp;
        qual_cap  = new_cap;
    }
}

/* Generate primes until we have at least 'count' primes in prime_list. */
static void generate_primes_up_to(idx_t count)
{
    if (prime_cnt >= count)
        return;

    uint64_t cand = prime_cnt ? (uint64_t)prime_list[prime_cnt - 1] + 1 : 2;
    while (prime_cnt < count) {
        bool isprime = true;
        for (idx_t i = 0; i < prime_cnt &&
             (uint64_t)prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % (uint64_t)prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = (seq_t)cand;
        }
        ++cand;
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1, but guard against 0 */
        return (seq_t)0;

    /* Produce qualifying indices until we have at least n of them. */
    while (qual_cnt < n) {
        /* Need the prime at position next_test_index. */
        generate_primes_up_to(next_test_index);

        /* Is the index itself prime? */
        if (is_prime_uint64((uint64_t)next_test_index)) {
            /* The next_test_index‑th prime is at prime_list[next_test_index-1]. */
            seq_t p_of_index = prime_list[next_test_index - 1];
            seq_t dsum = digit_sum_uint64((uint64_t)p_of_index);
            if (is_prime_uint64((uint64_t)dsum)) {
                /* Record this index. */
                ensure_qual_cap(qual_cnt + 1);
                qual_list[qual_cnt++] = next_test_index;
            }
        }
        ++next_test_index;
    }

    return (seq_t)qual_list[n - 1];
}
