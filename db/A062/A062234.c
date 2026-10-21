/*
 * A062234  From Bertrand's postulate: a(n) = 2*prime(n) - prime(n+1).
 * https://oeis.org/A062234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*prime(n) - prime(n+1)
 * Range:     u8 n <= 55, u16 n <= 6545, u32 n <= 9702, u64 n <= 9702,
 *            u128 n <= 9702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of primes – indexed from 0 (prime(1)=2). */
static uint32_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Append a new prime, growing the array if necessary. */
static void push_prime(uint32_t p)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint32_t *tmp = (uint32_t *)realloc(prime_arr, new_cap * sizeof(uint32_t));
        if (!tmp) abort();               /* allocation failure */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_cnt++] = p;
}

/* Ensure that at least 'need' primes are available (1‑based count). */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    if (prime_cnt == 0) {
        /* Start the sieve with the first prime 2. */
        push_prime(2);
        if (need == 1)
            return;

        uint32_t candidate = 3;          /* next odd candidate */
        while (prime_cnt < need) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = prime_arr[i];
                if ((uint64_t)p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime)
                push_prime(candidate);
            candidate += 2;               /* skip even numbers */
        }
    } else {
        /* Continue from the next odd number after the last prime. */
        uint32_t candidate = prime_arr[prime_cnt - 1] + 2;
        while (prime_cnt < need) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = prime_arr[i];
                if ((uint64_t)p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime)
                push_prime(candidate);
            candidate += 2;
        }
    }
}

/* Sequence function: a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for offset < 1 */

    /* Need primes up to index n+1. */
    ensure_primes((size_t)n + 1);

    seq_t p1 = (seq_t)prime_arr[n - 1]; /* prime(n)  */
    seq_t p2 = (seq_t)prime_arr[n];     /* prime(n+1)*/
    return p1 * 2 - p2;
}
