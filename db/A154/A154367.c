/*
 * A154367  Numbers k such that the sum of the prime factors of composite(k)
 *          (with multiplicity) is prime and lpf(composite(k)) +
 *          gpf(composite(k)) is composite.
 * https://oeis.org/A154367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Search for the n‑th integer k such that * sum of the prime
 *            factors (with multiplicity) of composite(k) * is prime and
 *            lpf(composite(k)) + gpf(composite(k)) is composite. * The
 *            sequence consists of those k values, OEIS A154367
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factor_sum_lpf_gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_sum_lpf_gpf.h"

/* ------------------------------------------------------------------ */
/* Small prime test --------------------------------------------------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Dynamic array helpers for the sequence values. ------------------ */
static seq_t *terms = NULL;   /* holds the k values that satisfy the property */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    if (terms_cap == 0) terms_cap = 16;
    while (terms_cap < needed) terms_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    terms = tmp;
}

/* ------------------------------------------------------------------ */
/* The sequence function -------------------------------------------- */
static uint64_t curr_num          = 4;   /* current integer being examined */
static idx_t    composite_index  = 0;   /* index (k) of the current composite */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* minimal safeguard */

    /* Extend the sequence until it contains at least n values */
    while (terms_len < (size_t)n) {
        /* Find next composite number */
        while (is_prime_u64(curr_num)) curr_num++;

        /* Increase composite counter: this coresponds to k */
        ++composite_index;

        /* Evaluate conditions for this composite */
        uint64_t sum, lpf, gpf;
        factor_sum_lpf_gpf(curr_num, &sum, &lpf, &gpf);

        bool sum_is_prime   = is_prime_u64(sum);
        bool lpf_plus_gpf_prime = is_prime_u64(lpf + gpf);

        if (sum_is_prime && !lpf_plus_gpf_prime) {
            /* Store the k value that satisfies the conditions */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)composite_index;
        }

        /* Move to next integer for further searching */
        ++curr_num;
    }

    return terms[(size_t)(n - 1)];
}
