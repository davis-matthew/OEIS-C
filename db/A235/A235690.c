/*
 * A235690  Semiprimes which have one or more occurrences of exactly two
 *          different digits.
 * https://oeis.org/A235690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   two_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A235690:
   semiprimes that use exactly two distinct decimal digits.
   The implementation searches the integers in increasing order,
   testing each for (1) semiprimality and (2) exactly two distinct
   digits. Results are memoised so that repeated calls are cheap. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/two_distinct_digits.h"

/* Prime sieve data */
static uint32_t *prime_list   = NULL;
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;

/* Ensure the prime list contains all primes <= limit */
static void extend_primes(idx_t limit)
{
    static idx_t current_limit = 0;
    if (limit <= current_limit) return;

    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;          /* out of memory – abort the search */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_cnt;

    if (new_cnt > prime_cap) {
        uint32_t *tmp = (uint32_t*)realloc(prime_list, new_cnt * sizeof(uint32_t));
        if (!tmp) { free(is_prime); return; }
        prime_list = tmp;
        prime_cap  = new_cnt;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    prime_count   = new_cnt;
    current_limit = limit;
    free(is_prime);
}

/* Integer square root */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Test whether x is a semiprime (product of exactly two primes,
   multiplicity counted) */
static bool is_semiprime(seq_t x)
{
    if (x < 4) return false;
    seq_t tmp = x;
    int    cnt = 0;

    idx_t limit = int_sqrt((idx_t)x);
    extend_primes(limit);

    for (size_t i = 0; i < prime_count &&
                         (seq_t)prime_list[i] * (seq_t)prime_list[i] <= tmp;
         ++i) {
        seq_t p = (seq_t)prime_list[i];
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (tmp > 1) ++cnt;

    return cnt == 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for zero index */

    /* memoisation */
    static seq_t *terms     = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t  next_cand  = 2;   /* next integer to test */

    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)next_cand;
        ++next_cand;

        if (is_semiprime(cand) && two_distinct_digits(cand)) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;       /* allocation failure */
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }
    }
    return terms[n - 1];
}
