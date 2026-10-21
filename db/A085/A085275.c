/*
 * A085275  a(n) = Sum of n-th composite number and its largest prime divisor.
 * https://oeis.org/A085275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n‑th composite number) + (largest prime divisor of that
 *            composite)
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------  Prime list helpers  --------- */
static uint64_t *prime_list   = NULL;  /* dynamic array of discovered primes */
static idx_t    prime_cnt     = 0;      /* number of primes stored   */
static idx_t    prime_cap     = 0;      /* allocated capacity       */

/* Add a newly found prime to the prime list */
static void add_prime(uint64_t p) {
    if (prime_cnt == prime_cap) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Test whether x is prime using the primes gathered so far.
   The caller must have already examined all integers <= x,
   so prime_list contains every prime <= x. */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Return the largest prime divisor of n.
   prime_list already contains all primes <= n. */
static uint64_t largest_prime_factor_uint64(uint64_t n) {
    uint64_t temp = n;
    uint64_t lpf   = 0;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p) break;
        if (temp % p == 0) {
            lpf = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) lpf = temp;          /* remaining prime factor */
    return lpf;
}

/* --------------  Main sequence function  -------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    idx_t   comp_cnt = 0;          /* number of composites seen */
    uint64_t cur     = 2;          /* integer to test */

    while (true) {
        /* Check if cur is prime.  If it is, remember it and skip it. */
        if (is_prime_uint64(cur)) {
            add_prime(cur);
            ++cur;
            continue;
        }

        /* cur is composite */
        ++comp_cnt;
        if (comp_cnt == n) {
            uint64_t lpf = largest_prime_factor_uint64(cur);
            seq_t  result = (seq_t)cur + (seq_t)lpf;
            return result;
        }

        ++cur;
    }
}
