/*
 * A180952  Primes such that p(i)+p(i+1)+p(i+2) is a prime when all addends
 *          are taken modulo 100.
 * https://oeis.org/A180952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180952: sequence of primes p such that (p % 100)+(next prime % 100)+(next next prime % 100) is prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* integer square root for idx_t (uint64_t) */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* static storage for primes and good primes */
static seq_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;

static seq_t *good = NULL;
static size_t good_len = 0, good_cap = 0;

static idx_t candidate = 1;   /* last checked number */

/* grow array to at least needed capacity */
static void grow_array(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);                       /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* check if n is prime using the primes list */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if ((n & 1) == 0)
        return false;
    idx_t r = isqrt_idx(n);
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p > r)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* append a prime to the primes list */
static void add_prime(idx_t p)
{
    grow_array(&primes, &primes_len, &primes_cap);
    primes[primes_len++] = (seq_t)p;

    /* If we have at least 3 primes, test the triple starting at len‑3 */
    if (primes_len >= 3) {
        seq_t p1 = primes[primes_len - 3];
        seq_t p2 = primes[primes_len - 2];
        seq_t p3 = primes[primes_len - 1];
        seq_t sum = (p1 % 100) + (p2 % 100) + (p3 % 100);
        if (is_prime((idx_t)sum)) {
            grow_array(&good, &good_len, &good_cap);
            good[good_len++] = p1;
        }
    }
}

/* generate primes until we have at least n good primes */
static void ensure_n_good(idx_t n)
{
    while (good_len < n) {
        /* find next candidate prime */
        do {
            ++candidate;
        } while (!is_prime_candidate(candidate));
        add_prime(candidate);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, but safe */
    ensure_n_good(n);
    return good[(size_t)(n - 1)];
}
