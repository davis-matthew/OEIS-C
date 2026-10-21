/*
 * A064051  a(n) = 2*prime(n)^2 - prime(n+1)^2.
 * https://oeis.org/A064051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*prime(n)^2 - prime(n+1)^2, where prime(k) is the k-th
 *            prime
 * Range:     i8 n <= 6, i16 n <= 42, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint32_t *prime_list = NULL;
static size_t prime_cnt  = 0;   /* number of primes already stored   */
static size_t prime_cap  = 0;   /* current capacity of prime_list    */

/* Ensure that prime_list contains at least 'needed' primes (1‑based). */
static void ensure_primes(size_t needed)
{
    if (needed <= prime_cnt)
        return;

    /* initialise storage if first call */
    if (prime_list == NULL) {
        prime_cap   = 16;
        prime_list  = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }

    uint32_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;

    while (prime_cnt < needed) {
        uint32_t limit = (uint32_t) sqrt((double) candidate);
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_primes((size_t)(n + 1));                   /* need prime(n) and prime(n+1) */

    seq_t p      = (seq_t)prime_list[n - 1];          /* prime(n)   */
    seq_t pnext  = (seq_t)prime_list[n];              /* prime(n+1) */

    return 2 * p * p - pnext * pnext;
}
