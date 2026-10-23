/*
 * A182673  a(n) = the smallest n-digit number with exactly 7 divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest n‑digit number of the form p⁶ (p prime), or
 *            0 if no such number exists
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* lower bound L = 10^(n-1) */
    seq_t L = pow10(n - 1);

    /* starting point: 10^floor((n-1)/6) */
    idx_t expLow = (n - 1) / 6;
    seq_t p = pow10(expLow);

    /* step until p⁶ >= L */
    seq_t pPow = big_pow(p, (seq_t)6);
    while (pPow < L) {
        ++p;
        pPow = big_pow(p, (seq_t)6);
    }

    /* search for the first prime ≥ p with exactly n digits */
    if (p < (seq_t)2) {
        p = (seq_t)2;
    }
    /* ensure p is odd if > 2 */
    if (p > (seq_t)2 && (p & (seq_t)1) == (seq_t)0) {
        ++p;
    }

    while (1) {
        seq_t power = big_pow(p, (seq_t)6);
        int digits = digit_len(power);
        if (digits > (int)n) {   /* exceeded the allowed digit count */
            break;
        }
        if (digits == (int)n && is_prime_big(p)) {
            return power;       /* found the smallest prime p */
        }
        p += (seq_t)2;           /* move to the next odd candidate */
    }

    return (seq_t)0;              /* no such number exists */
}
