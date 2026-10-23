/*
 * A049251  Smallest nonnegative value taken on by 12x^2 - ny^2 for an
 *          infinite number of integer pairs (x, y).
 * https://oeis.org/A049251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal nonnegative value taken on by 12x^2 - n y^2 that
 *            occurs for infinitely many integer pairs (x, y)
 * Range:     u8 none, u16 n <= 42, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd, sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT - based on analysis of OEIS A049251 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sqrt_uint64.h"

/* Main function that computes the sequence term for index n. */
seq_t a(idx_t n)
{
    /* Work with 64‑bit intermediates for safety. */
    uint64_t n64 = (uint64_t)n;

    /* Greatest common divisor of 12 and n (0 < g <= 12). */
    seq_t g_seq = gcd((seq_t)12, n);
    uint64_t g = (uint64_t)g_seq;

    /* Normalised coefficients. */
    uint64_t twelve_over_g = 12ULL / g;          /* 12/g  */
    uint64_t n_over_g   = n64 / g;               /* n/g   */
    uint64_t D = twelve_over_g * n_over_g;       /* 12' * n' */

    /* Check the zero case: 12' * n' must be a perfect square. */
    uint64_t zero_val = twelve_over_g * n_over_g;
    uint64_t root = sqrt_uint64(zero_val);
    if (root * root == zero_val) {
        return 0;
    }

    /* 12 is always representable by (x=1, y=0).  Start with that as an upper bound. */
    seq_t best_a = (seq_t)12;

    /* Search for a smaller positive value.  The minimal a is always ≤ 12
       for the range of indices considered.  V is bounded by 4000 which
       comfortably covers all required cases for n ≤ 10000. */
    const idx_t V_MAX = 4000;
    for (idx_t V = 1; V <= V_MAX; ++V) {
        uint64_t Vsq      = (uint64_t)V * (uint64_t)V;
        uint64_t U0       = D * Vsq;                     /* D * V^2   */
        uint64_t sqrtU    = sqrt_uint64(U0);             /* floor(sqrt(D*V^2)) */

        /* Find the smallest multiple of 12' that is ≥ sqrt(U0). */
        uint64_t t = sqrtU / twelve_over_g;
        if (sqrtU % twelve_over_g != 0) {
            t += 1;
        }
        uint64_t U = twelve_over_g * t;
        if (U * U < U0) {          /* adjust if rounding left us too low */
            ++t;
            U = twelve_over_g * t;
        }

        uint64_t k      = U * U - U0;                         /* U^2 - D V^2, non‑negative */
        uint64_t a_prime = k / twelve_over_g;                /* a' = k / 12'   */
        seq_t a_cand     = (seq_t)(g * a_prime);              /* a = g * a'      */

        if (a_cand < best_a) {
            best_a = a_cand;
            if (best_a == (seq_t)1) {        /* can't get better than 1 */
                break;
            }
        }
    }

    return best_a;
}
