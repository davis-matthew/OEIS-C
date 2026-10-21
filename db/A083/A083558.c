/*
 * A083558  p(p^2-p+1) as p runs through the primes.
 * https://oeis.org/A083558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) * (p(n)^2 - p(n) + 1), where p(n) is the nth prime
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *prime_list = NULL;     /* array of found primes */
static size_t  prime_cnt  = 0;       /* number of primes stored */
static size_t  prime_size = 0;       /* allocated capacity of prime_list */
static idx_t   last_checked = 1;     /* last integer tested for primality */

static int is_prime_candidate(idx_t candidate) {
    if (candidate < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > candidate / p) break;   /* p^2 > candidate */
        if (candidate % p == 0) return 0;
    }
    return 1;
}

static void ensure_nth_prime(idx_t n) {
    /* extend the prime list until it contains at least n primes */
    while (prime_cnt < n) {
        idx_t cand = last_checked + 1;
        last_checked = cand;
        if (is_prime_candidate(cand)) {
            if (prime_cnt >= prime_size) {
                size_t new_size = prime_size == 0 ? 16 : prime_size * 2;
                idx_t *new_list = (idx_t *)realloc(prime_list, new_size * sizeof(idx_t));
                if (!new_list) abort(); /* memory allocation failed */
                prime_list = new_list;
                prime_size = new_size;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* define a(0) as 0, though offset is 1 */
    ensure_nth_prime(n);
    idx_t p = prime_list[n-1];
    seq_t pp = (seq_t)p * (seq_t)p;                 /* p^2 */
    seq_t val = (seq_t)p * (pp - (seq_t)p + (seq_t)1);
    return val;
}
