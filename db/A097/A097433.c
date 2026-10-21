/*
 * A097433  Integer part of the hypotenuse of prime leg isosceles right
 *          triangles.
 * https://oeis.org/A097433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*p(n)^2))
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 1497, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prime generation – incremental list of primes.                       */
/* --------------------------------------------------------------------- */
static uint64_t *prime_buf       = NULL;   /* stored primes                  */
static size_t   prime_buf_size  = 0;      /* allocated buffer size          */
static size_t   prime_count     = 0;      /* number of primes currently stored */
static uint64_t prime_candidate = 2;      /* next candidate to test         */

/* Simple primality test using the primes already found.                 */
static inline bool is_prime_uint64(uint64_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_buf[i];
        if (p * p > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

/* Return the n‑th prime (1‑based). Generates primes as needed.           */
static inline uint64_t get_nth_prime_uint64(size_t n)
{
    while (prime_count < n) {
        /* find next prime */
        while (!is_prime_uint64(prime_candidate))
            ++prime_candidate;

        /* grow buffer if necessary */
        if (prime_count == prime_buf_size) {
            size_t new_size = prime_buf_size == 0 ? 16 : prime_buf_size << 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_buf, new_size * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_buf       = tmp;
            prime_buf_size  = new_size;
        }

        prime_buf[prime_count++] = prime_candidate;
        ++prime_candidate;
    }
    return prime_buf[n - 1];
}

/* --------------------------------------------------------------------- */
/*  Integer square‑root for the generic type seq_t.                      */
/*  Uses binary search with only + - * / % and comparisons.              */
/* --------------------------------------------------------------------- */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    const seq_t one = (seq_t)1;
    while (low < high) {
        seq_t mid = (low + high + one) >> 1;  /* (low+high+1)/2  */
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - one;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/*  Sequence function requested.                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0 */
    uint64_t p_val = get_nth_prime_uint64((size_t)n);
    seq_t p   = (seq_t)p_val;        /* nth prime */
    seq_t v   = (seq_t)2 * p * p;     /* 2 * p²   */
    return isqrt_seq(v);              /* floor sqrt */
}
