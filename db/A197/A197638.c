/*
 * A197638  GA1 numbers: composite m with G(m) >= G(m/p) for all prime factors
 *          p of m, where G(k) = sigma(k)/(k*log(log(k))) and sigma(k) = sum
 *          of divisors of k.
 * https://oeis.org/A197638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4; for n > 1 a(n) = 2 * p_{n-1}, where p_k is the k‑th
 *            prime greater than 5 (7, 11, 13, …)
 * Range:     u8 n <= 29, u16 n <= 3510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache of primes > 5 */
static seq_t *primes   = NULL;   /* array of primes > 5 */
static size_t primes_cnt = 0;    /* number of stored primes */
static size_t primes_cap = 0;    /* capacity of the array */

/*  integer primality test (6k±1) */
static inline bool is_prime_candidate(uint64_t n)
{
    if (n < 2)            return false;
    if (n <= 3)           return true;
    if ((n & 1) == 0)     return false;
    if (n % 3 == 0)       return false;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* generate primes > 5 until we have at least k of them */
static void ensure_primes_needed(idx_t k)
{
    if (primes_cnt >= (size_t)k)
        return;

    /* start searching from the last checked number
       (initially 5, so the first candidate will be 7) */
    static uint64_t last_checked = 5;
    uint64_t cand = last_checked + 1;
    if (cand < 7) cand = 7;
    if ((cand & 1) == 0) cand++;        /* only odd candidates needed */

    while (primes_cnt < (size_t)k) {
        if (is_prime_candidate(cand)) {
            /* grow the cache if necessary */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                seq_t *new_arr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                primes = new_arr;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (seq_t)cand;
        }
        cand += 2;                      /* next odd candidate */
    }
    last_checked = cand - 2;            /* last number that was tested */
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 1)      /* offset is 1 */
        return (seq_t)4;

    idx_t k = n - 1;                 /* index of prime > 5 */
    ensure_primes_needed(k);
    return (seq_t)2 * primes[k - 1];
}
