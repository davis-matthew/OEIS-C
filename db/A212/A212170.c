/*
 * A212170  a(n) = first counting number with n distinct positive square
 *          exponents in its prime factorization.
 * https://oeis.org/A212170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, if n = 1 a(n) = ∏_{i=0}^{n-2} prime_i ^ ((n-1-i)^2)
 *            for n > 1 where prime_i is the (i+1)‑th prime (prime_0 = 2)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided build_primes utility and big_pow. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* declares build_primes() */
#include "../utils/big_pow.h"       /* declares big_pow() */

/* --- prime cache --------------------------------------------------- */
static idx_t *prime_list = NULL;   /* array of primes (idx_t == uint64_t) */
static size_t prime_count = 0;     /* number of primes stored        */
static const idx_t PRIME_LIMIT = 200000;   /* > 17 000th prime */

/* build the prime list once, enough for all indices up to ~10000 */
static void ensure_primes(void)
{
    if (prime_list != NULL)
        return;   /* already initialised */

    build_primes(PRIME_LIMIT, &prime_list, &prime_count);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1)                /* a(1) = 1 */
        return (seq_t)1;

    /* For n > 1, we need n-1 distinct square exponents. */
    idx_t m = n - 1;           /* number of exponents needed */

    ensure_primes();           /* guarantee prime_list is ready */

    seq_t result = (seq_t)1;

    /* Multiply the required primes raised to the corresponding square
       exponents, assigning the largest exponent to the smallest prime. */
    for (idx_t i = 0; i < m; ++i) {
        idx_t exp   = (m - i) * (m - i);      /* square exponent */
        seq_t prime = (seq_t)prime_list[i];
        seq_t term  = big_pow(prime, (seq_t)exp);
        result *= term;
    }

    return result;
}
