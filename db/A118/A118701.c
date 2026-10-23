/*
 * A118701  Largest prime power that divides the average of twin-prime pairs.
 * https://oeis.org/A118701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime power dividing the average of the n‑th twin
 *            prime pair (average = p + 1 where (p, p+2) is the pair)
 * Range:     u8 n <= 17, u16 n <= 997, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;   /* list of primes */
static size_t prime_cnt   = 0;     /* number of primes */
static idx_t  prime_limit = 0;     /* highest bound used */

/* Ensure all primes up to 'limit' are generated */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;
    if (prime_list) free(prime_list);
    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Return the lower member of the n‑th twin‑prime pair (1‑based). */
static uint64_t nth_twin_prime_lower(idx_t n)
{
    if (n == 0) return 0;      /* safety guard – not used for n≥1 */
    idx_t limit = n * 20;      /* initial upper bound guess */
    if (limit < 100) limit = 100;
    while (1) {
        ensure_primes(limit);
        idx_t twin = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            idx_t q = prime_list[i + 1];
            if (q == p + 2) {
                ++twin;
                if (twin == n) return p;
            }
        }
        limit <<= 1;          /* not enough twins – double the bound */
    }
}

/* Return the largest prime power dividing x (x ≥ 2). */
static seq_t largest_prime_power(seq_t x)
{
    seq_t max_pow = 1;
    seq_t tmp = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            seq_t pow_val = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow_val *= p;
            }
            if (pow_val > max_pow) max_pow = pow_val;
        }
    }
    if (tmp > max_pow) max_pow = tmp;   /* remaining prime factor */
    return max_pow;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t lower = nth_twin_prime_lower(n);   /* lower twin prime */
    seq_t avg   = (seq_t)lower + 1;          /* average = p + 1 */
    return largest_prime_power(avg);
}
