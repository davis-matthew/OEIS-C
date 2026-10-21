/*
 * A094657  Primes congruent to 4 mod 17.
 * https://oeis.org/A094657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 406, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n-th prime that is ≡ 4 (mod 17),
   where a(1) is the first such prime (89).  The implementation
   keeps a dynamic list of all primes found so far and extracts
   those with the desired residue.  It works for any
   sequence type seq_t and index type idx_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void grow_uint64(uint64_t **arr, size_t *cap, size_t *cnt)
{
    if (*cnt >= *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

static bool is_prime_uint64(uint64_t x, const uint64_t *primes, size_t primes_cnt)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break; /* p*p > x without overflow */
        if (x % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint64_t *special = NULL;
    static size_t special_cnt = 0, special_cap = 0;
    static uint64_t last_checked = 1; /* start search from 2 */

    size_t target = (size_t)n;
    while (special_cnt < target) {
        /* find next prime after last_checked */
        uint64_t cand = last_checked + 1;
        while (!is_prime_uint64(cand, primes, primes_cnt)) {
            ++cand;
        }
        /* record the prime */
        grow_uint64(&primes, &primes_cap, &primes_cnt);
        primes[primes_cnt++] = cand;

        if ((cand % 17ull) == 4ull) {
            grow_uint64(&special, &special_cap, &special_cnt);
            special[special_cnt++] = cand;
        }

        last_checked = cand;
    }

    return (seq_t)special[(size_t)(n - 1)];
}
