/*
 * A133089  Expansion of f(x)^3 in powers of x where f() is a Ramanujan theta
 *          function.
 * https://oeis.org/A133089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(8*n+1) where b(m) is multiplicative: b(2^e) = 0 (e >
 *            0), b(p^e) = p^(e/2) if p≡1,3 (mod 8) and e even, b(p^e) =
 *            (-p)^(e/2) if p≡5,7 (mod 8) and e even
 * Range:     i8 n <= 7, i16 n <= 1853, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"          /* seq_t pow_seq(seq_t base, int exp); */
#include "../utils/floor_sqrt.h"      /* seq_t floor_sqrt(seq_t x); */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/* Static storage for the prime list. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* Ensure the prime list contains all primes up to at least 'limit'. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_list == NULL || limit > prime_limit)
    {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
        prime_limit = limit;
        build_primes_upto(limit, &prime_list, &prime_cnt);
    }
}

/* Compute the multiplicative function b(m) for unsigned 64‑bit m. */
static seq_t b_of_uint64(uint64_t m)
{
    if (m == 1) return (seq_t)1;

    seq_t  result = (seq_t)1;
    uint64_t temp = m;

    /* Find required prime bound and ensure primes are available. */
    idx_t bound = (idx_t)floor_sqrt((seq_t)m) + 1;
    ensure_primes_upto(bound);

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            if (p == 2)
            {
                /* b(2^e) = 0 for e > 0 */
                return (seq_t)0;
            }

            if (exp & 1ULL)   /* odd exponent => zero contribution */
                return (seq_t)0;

            int half = (int)(exp >> 1);  /* exp/2 fits in int for our limits */

            if (p % 8 == 1 || p % 8 == 3)
            {
                result *= pow_seq((seq_t)p, half);
            }
            else  /* p % 8 == 5 or 7 */
            {
                seq_t mult = pow_seq((seq_t)p, half);
                if (half & 1)  /* (-p)^(half) = -p^(half) when half odd */
                    mult = -mult;
                result *= mult;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1)
    {
        uint64_t p = temp;
        if (p == 2)
            return (seq_t)0;

        /* exponent is 1 - odd => zero contribution */
        /* But handle explicitly for clarity */
        /* Since exponent is 1 (odd), b(p) = 0 */
        return (seq_t)0;
    }

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n * 8ULL + 1ULL;
    return b_of_uint64(m);
}
