/*
 * A202388  Digital root of Smith numbers A006753.
 * https://oeis.org/A202388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digital_root( nth Smith number )
 * Range:     u8 n <= 9, u16 n <= 2375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A Smith number is a composite number whose sum of digits equals the
   sum of the digits of its prime factors (counting multiplicity). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

/*---------------------------------------------------------------------------*/
/* Helper: compute the sum of the digits of all prime factors of n,
   counted with multiplicity. */
static seq_t _prime_factor_digit_sum(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt  = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t d  = digit_sum((seq_t)primes[i]);      /* digit sum of the prime */
        sum += d * (seq_t)exps[i];                   /* multiplicity */
    }
    return sum;
}

/*---------------------------------------------------------------------------*/
/* Storage for the generated Smith numbers.                                 */
static seq_t *smiths      = NULL;   /* array of Smith numbers (as seq_t)   */
static size_t smiths_cap   = 0;     /* capacity of smiths array           */
static size_t smiths_len   = 0;     /* number of Smith numbers stored     */
static uint64_t curr_candidate = 4; /* next integer to test               */

/*---------------------------------------------------------------------------*/
/* Ensure that the first 'n' Smith numbers are generated and stored.         */
static void _ensure_smiths(idx_t n)
{
    while (smiths_len < (size_t)n) {
        uint64_t cand = curr_candidate;

        /* Skip primes – a Smith number must be composite. */
        if (!is_prime_small((seq_t)cand)) {
            seq_t sum_digits  = digit_sum((seq_t)cand);
            seq_t sum_factors = _prime_factor_digit_sum(cand);

            if (sum_digits == sum_factors) {
                /* Expand array if needed. */
                if (smiths_len == smiths_cap) {
                    smiths_cap = smiths_cap ? smiths_cap * 2 : 16;
                    smiths = (seq_t *)realloc(smiths, smiths_cap * sizeof(seq_t));
                }
                smiths[smiths_len++] = (seq_t)cand;
            }
        }
        ++curr_candidate;
    }
}

/*---------------------------------------------------------------------------*/
/* Public function.  Returns a(n) – the digital root of the n‑th Smith number. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for non‑positive indices */

    _ensure_smiths(n);

    seq_t smith = smiths[(size_t)n - 1];   /* nth Smith number */

    /* Compute the digital root by repeated digit summation. */
    seq_t dr = digit_sum(smith);
    while (dr >= (seq_t)10)
        dr = digit_sum(dr);

    return dr;
}
