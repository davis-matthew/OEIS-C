/*
 * A163122  Composite numbers for which the sum of proper divisors equals the
 *          sum of the digit-reversed proper divisors.
 * https://oeis.org/A163122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th composite number n with σ⁻(n) == revσ⁻(n), where
 *            σ⁻(n) is the sum of proper divisors and revσ⁻(n) is the sum of
 *            the decimal reversals of those divisors
 * Range:     u8 n <= 26, u16 n <= 324, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A163122 – composite numbers n such that
   sum of proper divisors = sum of digit‑reversed proper divisors.
   The function a(n) returns the n‑th element (1‑based indexing). */


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/rev.h"

#define LIMIT 1000000UL       /* enough to get many terms (≈>10 000) */

static uint64_t *sumproper = NULL;
static uint64_t *sumrev    = NULL;
static uint8_t *is_composite = NULL;
static seq_t *terms       = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

/* ----------------------------------------------------------------------*/
/*  Initialise arrays and compute the sequence up to LIMIT.               */
static void init(void)
{
    if (sumproper != NULL) return;   /* already initialised */

    /* Allocate zeroed arrays for sums of proper divisors and of
       reversed proper divisors. */
    sumproper = (uint64_t *)calloc((size_t)(LIMIT + 1), sizeof(uint64_t));
    sumrev    = (uint64_t *)calloc((size_t)(LIMIT + 1), sizeof(uint64_t));
    is_composite = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));

    /* Sieve of Eratosthenes – fill is_composite flags. */
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* Sum proper divisors:
       For every proper divisor i (i≥1), add i to all multiples j>i.
       Also add rev(i) to sumrev for those multiples. */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        uint64_t revv = (uint64_t)rev(i);   /* reverse of i */
        for (idx_t j = i << 1; j <= LIMIT; j += i) {
            sumproper[j] += (uint64_t)i;
            sumrev[j]    += revv;
        }
    }

    /* Allocate array for terms, starting small. */
    term_cap = 1024;
    terms = (seq_t *)malloc(term_cap * sizeof(seq_t));

    /* Collect composite numbers satisfying the condition. */
    for (idx_t n = 4; n <= LIMIT; ++n) {
        if (is_composite[n] && sumproper[n] == sumrev[n]) {
            if (term_count == term_cap) {
                term_cap <<= 1;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)n;
        }
    }
}

/* ----------------------------------------------------------------------*/
/*  Public sequence function.                                            */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    init();

    if (n == 0 || (size_t)n > term_count) return (seq_t)0;
    return terms[n - 1];
}
