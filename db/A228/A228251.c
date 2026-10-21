/*
 * A228251  Fundamental discriminant of least absolute value with class group
 *          of 2-rank n.
 * https://oeis.org/A228251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^k * 2 * A002110(n+1)
 *  where k = number of primes among the first (n+1) primes that are 3 (mod 4)
 *  A002110(n) is the primorial – product of the first n primes.
 *
 *  a(0) = -3.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic array of generated primes */
static uint64_t *primes = NULL;
static size_t     prime_count   = 0;   /* number of primes stored       */
static size_t     prime_capacity = 0;  /* allocated capacity of array    */

/* generate at least 'need' primes, extending the array if necessary   */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* grow capacity if needed */
    if (prime_capacity < need) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 8;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp)
            exit(1); /* allocation failed */
        primes = tmp;
        prime_capacity = new_cap;
    }

    /* if the array is empty, add prime 2 */
    if (prime_count == 0 && need > 0) {
        primes[prime_count++] = 2;
    }

    /* start searching from the first odd number after the last prime   */
    uint64_t candidate = primes[prime_count - 1] + 1;
    if ((candidate & 1ULL) == 0)
        ++candidate;   /* make it odd */

    for (; prime_count < need; candidate += 2) {        /* only odd candidates */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_count++] = candidate;
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-3;   /* special case */

    size_t needed = (size_t)(n + 1);   /* number of primes required */

    ensure_primes(needed);

    seq_t prod = 1;
    for (size_t i = 0; i < needed; ++i)
        prod *= (seq_t)primes[i];

    seq_t abs_val = (seq_t)2 * prod;

    /* count of primes ≡ 3 (mod 4) among the first n+1 primes,
       excluding the prime 2 which is at index 0                     */
    size_t count3mod4 = 0;
    for (size_t i = 1; i < needed; ++i)
        if ((primes[i] & 3ULL) == 3ULL)       /* p % 4 == 3 */
            ++count3mod4;

    /* sign = -1 if count is even, +1 if odd */
    int sign = (count3mod4 % 2 == 0) ? -1 : 1;

    return (seq_t)sign * abs_val;
}
