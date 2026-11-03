/*
 * A036504  Numerator of n^(n-1)/n!.
 * https://oeis.org/A036504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 237 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of n^(n-1)/n! */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;          /* used in Legendre's formula */
    idx_t temp = n;       /* temporary copy for factorisation */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e_n = 0;          /* exponent of p in n */
            while (temp % p == 0) {
                temp /= p;
                ++e_n;
            }

            /* exponent of p in n^(n‑1) */
            idx_t exp_num = (n - 1) * e_n;

            /* exponent of p in n! (Legendre) */
            idx_t leg = 0;
            idx_t div = p;
            while (div <= m) {
                leg += m / div;
                if (div > m / p)
                    break;          /* avoid overflow of div *= p */
                div *= p;
            }

            idx_t exp = exp_num > leg ? exp_num - leg : 0;
            if (exp > 0)
                result = seq_mul(result, ipow_idx((seq_t)p, exp));
        }
    }

    /* any remaining prime factor > sqrt(n) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e_n = 1;
        idx_t exp_num = (n - 1) * e_n;

        idx_t leg = 0;
        idx_t div = p;
        while (div <= m) {
            leg += m / div;
            if (div > m / p)
                break;
            div *= p;
        }

        idx_t exp = exp_num > leg ? exp_num - leg : 0;
        if (exp > 0)
            result = seq_mul(result, ipow_idx((seq_t)p, exp));
    }

    return result;
}
