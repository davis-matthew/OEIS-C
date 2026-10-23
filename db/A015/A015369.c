/*
 * A015369  Gaussian binomial coefficient [ n,8 ] for q=-12.
 * https://oeis.org/A015369
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} ((-12)^(n-i+1)-1)/((-12)^i-1)
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Gaussian binomial coefficient [n,8] for q = -12, for n >= 8. */
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;          /* undefined for indices below the offset */

    /* ------------------------------------------------------------------ */
    /* Step 1: Prepare magnitudes of numerators and denominators.         */
    /* ------------------------------------------------------------------ */
    seq_t pow_n = big_pow((seq_t)12, (seq_t)n);   /* 12^n               */
    seq_t numer_mag[8];
    bool numer_neg[8] = {0};

    for (idx_t i = 1; i <= 8; ++i) {
        idx_t exp = n - i + 1;           /* exponent of (-12)         */
        seq_t val = pow_n;               /* 12^exp, even | odd       */

        if (exp & 1) {                    /* odd exponent -> (-12)^exp negative */
            numer_neg[i-1] = true;
            numer_mag[i-1] = val + 1;     /* |-(12^exp)-1| = 12^exp+1    */
        } else {                          /* even exponent -> positive     */
            numer_neg[i-1] = false;
            numer_mag[i-1] = val - 1;     /* 12^exp-1                  */
        }

        pow_n /= (seq_t)12;               /* move to 12^(exp-1)           */
    }

    seq_t denom_mag[8];
    bool denom_neg[8] = {0};
    seq_t pow_d = 12;                     /* 12^1                         */

    for (idx_t i = 1; i <= 8; ++i) {
        seq_t val = pow_d;                /* 12^i                       */

        if (i & 1) {                     /* odd i -> (-12)^i negative   */
            denom_neg[i-1] = true;
            denom_mag[i-1] = val + 1;     /* |-(12^i)-1| = 12^i+1       */
        } else {                         /* even i -> positive          */
            denom_neg[i-1] = false;
            denom_mag[i-1] = val - 1;     /* 12^i-1   */
        }

        pow_d *= (seq_t)12;               /* advance to next power of 12  */
    }

    /* ------------------------------------------------------------------ */
    /* Step 2: Cancel common factors between numerators and denominators. */
    /* ------------------------------------------------------------------ */
    for (idx_t j = 0; j < 8; ++j) {
        for (idx_t i = 0; i < 8; ++i) {
            seq_t g = gcd(numer_mag[i], denom_mag[j]);
            if (g != (seq_t)1) {
                numer_mag[i] /= g;
                denom_mag[j] /= g;
                if (denom_mag[j] == (seq_t)1)
                    break;
            }
        }
    }

    /* Total sign should be +1; if any sign left we ignore because the    */
    /* product of all terms is known to be positive.                       */

    /* ------------------------------------------------------------------ */
    /* Step 3: Multiply the reduced numerators. (All denominators are 1)  */
    /* ------------------------------------------------------------------ */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 8; ++i) {
        result *= numer_mag[i];
    }

    return result;
}
