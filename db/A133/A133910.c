/*
 * A133910  Period numbers of A133900 divided by n^2.
 * https://oeis.org/A133910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Product_{p|n} p^(floor(log_p(n)) - v_p(n))
 * Range:     u8 n <= 29, u16 n <= 209, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the product is over the distinct prime divisors of n and v_p(n)
   is the exponent of p in the prime factorisation of n.  This is
   equivalent to a(n)=A133900(n)/n^2. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Compute the n-th term of sequence A133910.
   n is indexed from 1 (a(1)=1). */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1. */
    if (n <= 1U) {
        return (seq_t)1;
    }

    /* Work with a 64‑bit unsigned integer for factorisation. */
    uint64_t nn   = (uint64_t)n;   /* original n */
    uint64_t temp = nn;           /* remaining part to factor */
    seq_t     result = (seq_t)1;   /* accumulating product */

    /* Special‑case factor 2. */
    uint64_t s = 0;                /* exponent of 2 in n */
    while ((temp & 1U) == 0U) {
        temp >>= 1U;
        s++;
    }
    if (s) {
        /* Compute k = floor(log_2(nn)). */
        uint64_t k = 0;
        uint64_t p = 1U;
        while (p <= nn / 2U) {
            p <<= 1U;
            k++;
        }
        if (k > s) {
            seq_t exp = (seq_t)(k - s);
            result *= big_pow((seq_t)2U, exp);
        }
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3U; p <= temp / p; p += 2U) {
        if (temp % p != 0U) {
            continue;
        }
        /* Count exponent s of prime p. */
        uint64_t s_p = 0;
        while (temp % p == 0U) {
            temp /= p;
            s_p++;
        }

        /* Compute k = floor(log_p(nn)). */
        uint64_t k = 0;
        uint64_t pp = 1U;
        while (pp <= nn / p) {
            pp *= p;
            k++;
        }

        if (k > s_p) {
            seq_t exp = (seq_t)(k - s_p);
            result *= big_pow((seq_t)p, exp);
        }
    }

    /* If temp > 1, it is a remaining prime factor. */
    if (temp > 1U) {
        uint64_t p = temp;
        uint64_t s_p = 1U;

        uint64_t k = 0;
        uint64_t pp = 1U;
        while (pp <= nn / p) {
            pp *= p;
            k++;
        }

        if (k > s_p) {
            seq_t exp = (seq_t)(k - s_p);
            result *= big_pow((seq_t)p, exp);
        }
    }

    return result;
}
