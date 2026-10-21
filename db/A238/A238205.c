/*
 * A238205  a(n) is the smallest prime such that 2n=p1+p2*p3 where
 *          Max(p1,p2,p3)=a(n) and p1, p2 p3 are odd primes.
 * https://oeis.org/A238205
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest prime p such that 2n = p1 + p2*p3 where p1,
 *            p2, p3 are odd primes and max(p1,p2,p3) = p
 * Range:     u8 n <= 2530, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Static data for prime table */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static bool   *prime_flag = NULL;
static idx_t  prime_flag_size = 0;

/* Ensure prime_flag and prime_list up to 'limit' (inclusive) */
static void ensure_prime_table(idx_t limit)
{
    if (prime_flag_size > limit)
        return;               /* already computed enough */

    /* Allocate new flag array   */
    bool *new_flag = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!new_flag) return;    /* allocation failed, but we still keep old */

    /* Sieve of Eratosthenes (odd primes only) */
    new_flag[0] = new_flag[1] = true;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (!new_flag[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_flag[j] = true;

    /* Count odd primes */
    size_t new_cnt = 0;
    for (idx_t i = 3; i <= limit; i += 2)
        if (!new_flag[i]) ++new_cnt;

    /* Build prime list */
    uint64_t *new_primes = NULL;
    if (new_cnt > 0) {
        new_primes = (uint64_t*)malloc(new_cnt * sizeof(uint64_t));
        size_t pos = 0;
        for (idx_t i = 3; i <= limit; i += 2)
            if (!new_flag[i]) new_primes[pos++] = i;
    }

    /* Replace old data and clean up */
    free(prime_flag);
    free(prime_list);
    prime_flag       = new_flag;
    prime_flag_size  = limit + 1;
    prime_list       = new_primes;
    prime_cnt        = new_cnt;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 6)          /* undefined for n < 6; return 0 for safety */
        return (seq_t)0;

    idx_t target = 2 * n;               /* 2n */
    ensure_prime_table(target);

    uint64_t best = UINT64_MAX;
    bool done = false;

    for (size_t i = 0; i < prime_cnt && !done; ++i) {
        uint64_t p2 = prime_list[i];
        if (p2 > target) break;

        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p3 = prime_list[j];
            uint64_t prod = p2 * p3;
            if (prod > target - 3) break;          /* p1 must be at least 3 */

            uint64_t p1 = target - prod;
            if (p1 < 3) continue;
            if (p1 >= prime_flag_size || prime_flag[p1]) continue;  /* not prime */

            /* compute maximum of the triple */
            uint64_t mx = p1;
            if (p2 > mx) mx = p2;
            if (p3 > mx) mx = p3;

            if (mx < best) {
                best = mx;
                if (best == 3) { done = true; break; }   /* smallest possible */
            }
        }
    }

    if (best == UINT64_MAX)   /* should not happen for n >= 6 */
        return (seq_t)0;

    return (seq_t)best;
}
