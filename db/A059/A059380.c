/*
 * A059380  Array of values of Jordan function J_k(n) read by antidiagonals
 *          (version 2).
 * https://oeis.org/A059380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = J_{k}(j) where let s = ceil((1 + sqrt(1 + 8*n)) / 2) (sum
 *            of indices) let j = n - (s-1)(s-2)/2 (column index) let k = s -
 *            j (row index) J_k(j) = j^k ∏_{p|j}(1 - 1/p^k) (Jordan totient of
 *            order k)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   jordan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jordan.h"

static idx_t find_s(idx_t n)
{
    /* minimal s with s*(s-1)/2 >= n */
    idx_t low = 2, high = 2;
    while ((__int128)high * (high - 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* generate primes up to limit (inclusive), replacing existing list */
static void ensure_primes(size_t limit,
       uint64_t **primes, size_t *cnt, size_t *prime_limit)
{
    if (limit <= *prime_limit)
        return;
    /* sieve up to new limit */
    size_t new_limit = (*prime_limit > 0) ? *prime_limit * 2 : 2;
    while (new_limit < limit)
        new_limit <<= 1;

    bool *is_composite = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!is_composite) exit(1);
    for (size_t i = 0; i <= new_limit; ++i) is_composite[i] = false;

    size_t max_cnt = new_limit / 10 + 10;          /* generous estimate */
    uint64_t *new_list = (uint64_t*)malloc(max_cnt * sizeof(uint64_t));
    if (!new_list) exit(1);
    size_t new_cnt = 0;

    for (size_t p = 2; p <= new_limit; ++p) {
        if (!is_composite[p]) {
            if (new_cnt >= max_cnt) {               /* grow if needed */
                max_cnt *= 2;
                new_list = (uint64_t*)realloc(new_list, max_cnt * sizeof(uint64_t));
                if (!new_list) exit(1);
            }
            new_list[new_cnt++] = p;
            if (p * p <= new_limit)
                for (size_t m = p * p; m <= new_limit; m += p)
                    is_composite[m] = true;
        }
    }
    free(is_composite);

    free(*primes);
    *primes = new_list;
    *cnt = new_cnt;
    *prime_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but safe */

    idx_t s = find_s(n);
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t pos = n - prev;
    idx_t j = pos;          /* column index */
    idx_t k = s - pos;      /* row index */

    /* static prime list */
    static uint64_t *prime_list = NULL;
    static size_t    prime_count = 0;
    static size_t    prime_limit = 0;

    ensure_primes((size_t)j, &prime_list, &prime_count, &prime_limit);

    return jordan(k, j, prime_list, prime_count);
}
