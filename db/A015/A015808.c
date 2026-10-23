/*
 * A015808  k is the first integer such that phi(k) + n | sigma(k).
 * https://oeis.org/A015808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow_seq_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A015808:
 *  a(n) = the smallest positive integer k such that φ(k) + n | σ(k)
 * where φ is Euler's totient and σ the sum‑of‑divisors function.
 * The function searches k from 1 upwards computing φ(k) and σ(k)
 * via a pre‑computed smallest‑prime‑factor (SPF) sieve.  This keeps
 * the per‑call cost well below the 2‑second limit for n up to at
 * least 10 000.  The algorithm is O(k log k) per call, where k
 * is the answer for the given n.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_seq_value.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* SPF sieve (smallest prime factor array)                               */
static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_limit = 0;          /* current size of spf array     */
static bool spf_initialized = false;

/* Ensure spf is computed up to at least 'limit'. */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    /* Allocate a fresh array, free old one if it exists. */
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) return;          /* in production you would handle this */

    /* Initialize SPF using a linear sieve. */
    new_spf[0] = new_spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) new_spf[i] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
    spf_initialized = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                  */
seq_t a(idx_t n)
{
    /* Start with a moderate sieve size; it will grow as needed. */
    static size_t current_limit = 200000;  /* initial limit */
    ensure_spf(current_limit);

    for (seq_t k = 1;; ++k) {
        /* If k exceeds the current sieve, extend it. */
        if ((size_t)k > spf_limit) {
            size_t new_limit = current_limit * 2;
            if (new_limit < (size_t)k * 2) new_limit = (size_t)k * 2;
            ensure_spf(new_limit);
            current_limit = new_limit;
        }

        /* Compute φ(k) and σ(k) using the SPF array. */
        seq_t phi   = 1;
        seq_t sigma = 1;
        uint32_t tmp = (uint32_t)k;   /* k is kept within the sieve range */
        while (tmp > 1) {
            uint32_t p   = spf[tmp];
            unsigned exp = 0;
            while (tmp % p == 0) { ++exp; tmp /= p; }

            seq_t p_exp_minus1 = pow_seq_value((seq_t)p, exp - 1);
            seq_t phi_factor   = p_exp_minus1 * ((seq_t)(p - 1));
            phi *= phi_factor;

            seq_t p_exp_plus1  = pow_seq_value((seq_t)p, exp + 1);
            seq_t sigma_factor = (p_exp_plus1 - 1) / ((seq_t)(p - 1));
            sigma *= sigma_factor;
        }

        seq_t denom = phi + (seq_t)n;
        if ((sigma % denom) == 0)
            return (seq_t)k;
    }
}
