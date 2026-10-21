/*
 * A231231  Numbers m such that, in the prime factorization of m, the product
 *          of the exponents equals the sum of prime factors and exponents.
 * https://oeis.org/A231231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A231231 by brute‑force search.
   a(n) = n-th integer m such that:
       (product of the exponents in the prime factorization of m)
       equals
       (sum of each prime factor of m plus its exponent). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Build a list of primes up to a fixed limit. */
#include "../utils/build_primes_upto.h"

/* Global prime list (up to 2 000 000). */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt = 0;

/* Initialize the prime list once. */
static void init_primes(void)
{
    if (!prime_list) {
        size_t cnt;
        /* 2 000 000 is well beyond the square root of the 10 000th
           term of this sequence; adjust if needed. */
        build_primes_upto(2000000ULL, &prime_list, &cnt);
        prime_cnt = cnt;
    }
}

/* Factor a 64‑bit integer into primes and exponents.
   Arrays pf[] and ex[] must be at least 64 elements long. */
static void factor_uint64(uint64_t n,
                          uint64_t pf[64], uint64_t ex[64], size_t *cnt)
{
    size_t i;
    uint64_t t = n;
    size_t c = 0;
    for (i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= t; ++i) {
        uint64_t p = prime_list[i];
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            pf[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (t > 1) {            /* remaining prime factor */
        pf[c] = t;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Public function required by the evaluator. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for the offset */
        return 0;

    init_primes();

    idx_t found = 0;           /* count of terms found so far */
    uint64_t m = 2;            /* candidate integer to test */

    /* Keep scanning until we have found the nth term. */
    while (true) {
        uint64_t pf[64], ex[64];
        size_t cnt;
        factor_uint64(m, pf, ex, &cnt);

        if (cnt > 0) {
            unsigned __int128 prod = 1;
            uint64_t sum = 0;
            for (size_t i = 0; i < cnt; ++i) {
                prod *= (unsigned __int128)ex[i];
                sum += pf[i] + ex[i];
            }
            if (prod == (unsigned __int128)sum) {
                ++found;
                if (found == n)
                    return (seq_t)m;   /* cast handles builtin or arbitrary‑precision types */
            }
        }
        ++m;
    }
}
