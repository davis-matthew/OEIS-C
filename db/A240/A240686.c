/*
 * A240686  Number of forests with n labeled nodes and 9 trees.
 * https://oeis.org/A240686
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-18) * (n-8)(n-7)(n-6)(n-5)(n-4)(n-3)(n-2)(n-1) *
 *            (n^8 + 76n^7 + 3122n^6 + 88760n^5 + 1873921n^4 + 29555596n^3 +
 *            334746252n^2 + 2455095600n + 8821612800) / 10321920
 * Range:     u8 none, u16 none, u32 none, u64 n <= 21, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Helper: multiply a by factor f, cancelling common factors with denom.
   After the call denom is reduced, and *res holds the updated result. */
static inline void apply_factor(seq_t *res, seq_t *denom, seq_t f)
{
    seq_t g = gcd(f, *denom);
    if (g > 1) {
        *denom /= g;
        f /= g;
    }
    *res *= f;
    /* Reduce any common factor that may now appear between res and denom */
    seq_t g2 = gcd(*res, *denom);
    if (g2 > 1) {
        *res /= g2;
        *denom /= g2;
    }
}

seq_t a(idx_t n)
{
    if (n < 9) return 0;            /* offset is 9 */

    seq_t denom = (seq_t)10321920;

    /* If n < 18 we must divide by n^(18-n); multiply denom by those factors */
    if (n < 18) {
        idx_t exp = 18 - n;
        for (idx_t i = 0; i < exp; ++i)
            denom *= (seq_t)n;
    }

    seq_t result = 1;

    /* multiply (n-8)(n-7)...(n-1) */
    for (idx_t j = n - 8; j <= n - 1; ++j) {
        apply_factor(&result, &denom, (seq_t)j);
    }

    /* evaluate the degree‑8 polynomial in n via Horner */
    const seq_t coeffs[9] = {
        (seq_t)1ULL,
        (seq_t)76ULL,
        (seq_t)3122ULL,
        (seq_t)88760ULL,
        (seq_t)1873921ULL,
        (seq_t)29555596ULL,
        (seq_t)334746252ULL,
        (seq_t)2455095600ULL,
        (seq_t)8821612800ULL
    };
    seq_t poly = coeffs[0];
    for (int k = 1; k < 9; ++k) {
        poly = poly * (seq_t)n + coeffs[k];
    }
    apply_factor(&result, &denom, poly);

    /* multiply by n^(n-18) when n >= 18 */
    if (n >= 18) {
        idx_t exp = n - 18;
        for (idx_t i = 0; i < exp; ++i) {
            apply_factor(&result, &denom, (seq_t)n);
        }
    }

    /* any leftover division factor (should be 1) */
    if (denom != (seq_t)1) {
        result /= denom;
    }

    return result;
}
