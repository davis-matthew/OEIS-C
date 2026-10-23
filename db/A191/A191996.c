/*
 * A191996  Numerators of partial products of a Hardy-Littlewood constant.
 * https://oeis.org/A191996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of r(n) * where r(n) = 2 * Π_{j=2..n} (1 -
 *            1/(p_j-1)^2) (p_j is the j‑th prime)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* offset starts at 1, so a(1) = 2 */
    if (n == 0) return 0;          /* out of scope, safety */
    if (n == 1) return (seq_t)2;

    /* obtain the first n primes: primes[0] = 2, primes[1] = 3, ... */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_first_n_primes(n, &primes, &prime_cnt);
    if (prime_cnt != (size_t)n) {
        free(primes);
        return 0;                  /* should not happen */
    }

    seq_t num = (seq_t)2;          /* current numerator (reduced form) */
    seq_t den = (seq_t)1;          /* current denominator (reduced form) */

    /* product over j = 2 .. n  ==> primes[1] .. primes[n-1] */
    for (idx_t j = 1; j < n; ++j) {   /* primes[j] is the j+1‑th prime */
        seq_t p       = (seq_t)primes[j];
        seq_t term_num = p * (p - (seq_t)2);      /* p*(p-2)  */
        seq_t term_den = (p - (seq_t)1) * (p - (seq_t)1); /* (p-1)^2 */

        /* cross‑reduce to keep numbers small */
        seq_t g1 = gcd(num, term_den);
        if (g1 > 1) {
            num      /= g1;
            term_den  /= g1;
        }

        seq_t g2 = gcd(term_num, den);
        if (g2 > 1) {
            term_num /= g2;
            den      /= g2;
        }

        /* multiply, using an intermediate wider type to avoid overflow */
        #if defined(__SIZEOF_INT128__)
        __uint128_t prod_num = (__uint128_t)num * ( __uint128_t)term_num;
        __uint128_t prod_den = (__uint128_t)den * ( __uint128_t)term_den;
        #else
        /* fall back to 64‑bit intermediate if 128‑bit not available */
        #  if UINT_MAX == 0xFFFFFFFFu
        unsigned long long prod_num = (unsigned long long)num * (unsigned long long)term_num;
        unsigned long long prod_den = (unsigned long long)den * (unsigned long long)term_den;
        #  else
        /* for 32‑bit seq_t */
        unsigned long prod_num = (unsigned)num * (unsigned)term_num;
        unsigned long prod_den = (unsigned)den * (unsigned)term_den;
        #  endif
        #endif

        num = (seq_t)prod_num;
        den = (seq_t)prod_den;
    }

    free(primes);          /* clean up */
    return num;            /* numerator of the reduced fraction */
}
