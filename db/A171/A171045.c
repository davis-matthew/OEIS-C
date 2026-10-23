/*
 * A171045  In the sequence of prime numbers, replace all the '8' digits with
 *          '4' and vice versa.
 * https://oeis.org/A171045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171045: replace every digit 8 in the nth prime with 4
   and every 4 with 8.  The algorithm builds a cache of primes
   incrementally (trial division) so that each term can be computed
   in O(n) time and O(n) memory. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t swap_digits(seq_t x)
{
    seq_t result = 0;
    seq_t mul     = 1;
    while (x > 0) {
        seq_t dig = x % 10;
        x /= 10;
        seq_t newdig = (dig == 4 ? 8 : (dig == 8 ? 4 : dig));
        result += newdig * mul;
        mul *= 10;
    }
    return result;
}

/* Prime cache */
static uint64_t *prime_cache = NULL;   /* stored primes */
static idx_t    prime_cap   = 0;       /* allocated capacity */
static idx_t    prime_cnt   = 0;       /* number of primes stored */
static uint64_t  checked_up_to = 0;    /* largest number checked for primality */

/* Grow the prime cache, ensuring at least new_cap elements */
static void grow_prime_cache(idx_t new_cap)
{
    if (prime_cap >= new_cap)
        return;
    idx_t newsize = prime_cap ? prime_cap : 16;
    while (newsize < new_cap)
        newsize *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                        newsize * sizeof(uint64_t));
    if (!tmp)
        exit(1);                     /* out of memory – allowed */
    prime_cache = tmp;
    prime_cap   = newsize;
}

/* Ensure that the cache contains at least n primes (1‑based). */
static void ensure_primes_upto(idx_t n)
{
    if (prime_cnt >= n)
        return;

    grow_prime_cache(n);

    /* Start from the next number after the last checked one. */
    uint64_t cand = checked_up_to + 1;
    if (cand < 2)
        cand = 2;

    for (; prime_cnt < n; ++cand) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_cnt++] = cand;
        }
    }
    checked_up_to = prime_cache[prime_cnt - 1];
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based; n≥1 in practice */
        return 0;
    ensure_primes_upto(n);
    uint64_t prime_val = prime_cache[n - 1];
    seq_t prime_seq = (seq_t)prime_val;
    return swap_digits(prime_seq);
}
