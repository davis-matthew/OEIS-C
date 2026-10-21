/*
 * A098015  Indices x such that (1/2)(prime(x+1) - prime(x)) is prime.
 * https://oeis.org/A098015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = index of the prime p_k such that (prime(k+1)-prime(k))/2
 *            is prime
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether n is prime using all known primes up to sqrt(n).
   The array 'primes' contains all primes found so far. */
static bool is_prime_uint64(uint64_t n, const uint64_t *primes, size_t prime_len)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* storage for discovered primes */
    static uint64_t *prime_list = NULL;
    static size_t prime_len = 0, prime_cap = 0;

    /* storage for the indices of good primes */
    static seq_t *good = NULL;
    static size_t good_len = 0, good_cap = 0;

    /* last prime found and its 1‑based index */
    static uint64_t prevPrime = 0;
    static idx_t prevIndex = 0;

    /* next candidate to test for primality */
    static uint64_t cand = 2;

    /* Initialise static storage on first call */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (good_cap == 0) {
        good_cap = 8;
        good = (seq_t *)malloc(good_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n good indices */
    while (good_len < (size_t)n) {
        /* Find next prime */
        while (true) {
            if (is_prime_uint64(cand, prime_list, prime_len))
                break;
            cand += (cand == 2) ? 1 : 2;          /* 2 → 3, otherwise skip evens */
        }

        /* Store the newly found prime */
        if (prime_len == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_len++] = cand;

        /* If we have a previous prime, check the gap */
        if (prevPrime != 0) {
            uint64_t diff = cand - prevPrime;
            if ((diff & 1ULL) == 0) {               /* diff even */
                uint64_t half = diff >> 1;          /* (p_{k+1}-p_k)/2 */
                if (is_prime_uint64(half, prime_list, prime_len)) {
                    if (good_len == good_cap) {
                        good_cap *= 2;
                        good = (seq_t *)realloc(good, good_cap * sizeof(seq_t));
                    }
                    /* Record the index of the earlier prime (prevIndex) */
                    good[good_len++] = (seq_t)prevIndex;
                }
            }
        }

        /* Update state for the next iteration */
        prevPrime = cand;
        prevIndex = (idx_t)prime_len;            /* indices are 1‑based */
        cand += (cand == 2) ? 1 : 2;             /* advance to next candidate */
    }

    return good[(size_t)n - 1];
}
