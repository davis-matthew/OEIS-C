/*
 * A181516  Primes in A011371.
 * https://oeis.org/A181516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3378, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A181516
 *  Primes p such that 2^p divides some factorial m!,
 *  but 2^(p+1) does not divide m!.
 *  We generate successive m and record each unique prime
 *  value of v2(m!) = m - popcount(m).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* OEIS helper: tests primality of idx_t values. */
#include "../utils/is_prime.h"

/* Function returning the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order calls */
    static seq_t *primes    = NULL;   /* array of found primes      */
    static size_t primes_len = 0;     /* number of primes stored    */
    static size_t primes_cap = 0;     /* current capacity of array  */
    static idx_t   processed_m = 1;   /* next m to examine          */

    /* grow array if needed */
    if (!primes) {
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) abort();
    }

    /* generate until we have at least n primes */
    while (primes_len < (size_t)n) {
        idx_t m   = processed_m++;
        idx_t v   = m - (idx_t)__builtin_popcountll(m); /* v2(m!)  */
        if (is_prime(v)) {
            /* store only the first occurrence of each prime */
            if (primes_len == 0 || (seq_t)primes[primes_len-1] < (seq_t)v) {
                if (primes_len == primes_cap) {
                    primes_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    primes = tmp;
                }
                primes[primes_len++] = (seq_t)v;
            }
        }
    }

    return primes[n-1];
}
