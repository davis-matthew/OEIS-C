/*
 * A060325  a(n) = n-th prime prime(n) subtracted from sum of all composites
 *          between prime(n) and prime(n-1).
 * https://oeis.org/A060325
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((p_{n-1} + p_n) * (p_n - p_{n-1} - 1)) / 2 - p_n where
 *            p_k is the k‑th prime and n ≥ 2
 * Range:     i8 n <= 11, i16 n <= 217, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sum of all composites between the two
   consecutive primes p_{n-1} and p_n and then subtracts p_n.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamically grow an array of the first k primes.  */
static uint32_t *primes   = NULL;   /* prime numbers */
static size_t   primes_cnt = 0;     /* number of primes stored */
static size_t   primes_cap = 0;     /* capacity of the array */

/* Ensure that primes contains at least n primes (n ≥ 1).  */
static void ensure_primes(size_t n)
{
    if (primes_cnt >= n)
        return;

    /* Grow the array if necessary. */
    if (primes_cap < n) {
        size_t new_cap = primes_cap ? (primes_cap << 1) : 64;
        while (new_cap < n)
            new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp)
            abort();
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Generate primes incrementally. */
    uint32_t cur = (primes_cnt ? primes[primes_cnt - 1] + 1 : 2);
    while (primes_cnt < n) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > cur)
                break;
            if (cur % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_cnt++] = cur;
        ++cur;
    }
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values for n < 2 are irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Use a size_t index for the prime array (n ≤ 10⁴ in tests). */
    size_t idx = (size_t)n;
    ensure_primes(idx);

    /* Retrieve consecutive primes.  */
    seq_t p_prev = (seq_t)primes[idx - 2];  /* prime(n-1) */
    seq_t p_curr = (seq_t)primes[idx - 1];  /* prime(n)   */

    /* Sum of all composites between p_prev and p_curr. */
    seq_t gap   = p_curr - p_prev;                     /* number of gaps */
    seq_t sum   = (p_prev + p_curr) * (gap - 1) / 2;   /* arithmetic series */
    return sum - p_curr;                               /* subtract prime(n) */
}
