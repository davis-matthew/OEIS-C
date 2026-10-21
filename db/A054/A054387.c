/*
 * A054387  Numerators of coefficients of 1/2^(2n+1) in Newton's series for
 *          Pi.
 * https://oeis.org/A054387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 22, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   exp_in_number, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = numerator of ( (2n‑2)! / ((n‑1)!² • (2n‑3) • (2n+1) • 2^(4n‑2) ) )
 * reduced to lowest terms.  The sign is positive for n≥2; a(0)=0, a(1)=−2.
 */

#include <stdint.h>    /* for uint32_t */
#include <stddef.h>    /* for size_t */
#include <stdbool.h>   /* for bool */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */
#include "../utils/exp_in_number.h"

/* maximum 2n‑2 needed for n up to 10 000 */
#define MAX_PRIME 20000

static uint32_t primes[MAX_PRIME + 1];
static size_t  primes_cnt   = 0;
static bool    primes_ready = false;

/* exponent of prime p in m! (Legendre's formula) */
static inline idx_t exp_in_factorial(idx_t m, uint32_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)-2;

    /* one‑time sieve of primes ≤ MAX_PRIME */
    if (!primes_ready) {
        bool is_comp[MAX_PRIME + 1] = {false};
        for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
            if (!is_comp[i]) {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                    is_comp[j] = true;
            }
        }
        for (idx_t i = 2; i <= MAX_PRIME; ++i)
            if (!is_comp[i]) primes[primes_cnt++] = (uint32_t)i;
        primes_ready = true;
    }

    idx_t two_n_minus_2 = 2 * n - 2;
    seq_t result = 1;

    for (size_t pi = 0; pi < primes_cnt && primes[pi] <= two_n_minus_2; ++pi) {
        uint32_t p = primes[pi];

        /* exponents in the binomial coefficient C(2n-2, n-1) */
        idx_t eC = exp_in_factorial(two_n_minus_2, p)
                 - 2 * exp_in_factorial(n - 1, p);

        /* exponents in the denominator (2n-3)(2n+1)2^(4n-2) */
        idx_t eD = exp_in_number(2 * n - 3, p)
                 + exp_in_number(2 * n + 1, p);
        if (p == 2) eD += (4 * n - 2);

        /* exponent that survives in the reduced numerator */
        if (eC > eD) {
            idx_t e = eC - eD;
            seq_t power = seq_pow((seq_t)p, e);
            result *= power;
        }
    }

    return result;
}
