/*
 * A015393  Gaussian binomial coefficient [ n,10 ] for q=-7.
 * https://oeis.org/A015393
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..10} ((-7)^(n-i+1)-1)/((-7)^i-1)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

static seq_t denom[10];
static bool denom_inited = false;

/* Return the Gaussian binomial coefficient [n,10] for q = -7.  */
seq_t a(idx_t n)
{
    /* For indices below the offset (10) the behaviour is irrelevant. */
    if (n < (idx_t)10) return (seq_t)0;

    /* Pre‑compute denominator values once. */
    if (!denom_inited) {
        for (idx_t i = 1; i <= (idx_t)10; ++i) {
            seq_t pow7 = seq_pow((seq_t)7, i);
            if (i & 1)   /* (-7)^i - 1 = -(7^i + 1) → absolute value = 7^i + 1 */
                denom[i-1] = pow7 + (seq_t)1;
            else        /* (-7)^i - 1 = 7^i - 1 */
                denom[i-1] = pow7 - (seq_t)1;
        }
        denom_inited = true;
    }

    /* Compute numerators for the requested n. */
    seq_t num[10];
    for (idx_t i = 0; i < (idx_t)10; ++i) {
        idx_t e = n - i;                  /* e = n - (i+1) + 1 = n - i */
        seq_t pow7 = seq_pow((seq_t)7, e);
        if (e & 1)   /* (-7)^e - 1 = -(7^e + 1)  → abs = 7^e + 1 */
            num[i] = pow7 + (seq_t)1;
        else        /* (-7)^e - 1 = 7^e - 1 */
            num[i] = pow7 - (seq_t)1;
    }

    /* Exact cancellation of each denominator factor with the numerators. */
    for (idx_t d_idx = 0; d_idx < (idx_t)10; ++d_idx) {
        seq_t d = denom[d_idx];
        if (d <= (seq_t)1) continue;
        for (idx_t n_idx = 0; n_idx < (idx_t)10 && d > (seq_t)1; ++n_idx) {
            seq_t g = seq_gcd(num[n_idx], d);
            if (g > (seq_t)1) {
                num[n_idx] /= g;
                d /= g;
            }
        }
        /* After cancellation d should be 1 for a correct definition. */
    }

    /* Multiply the remaining numerator factors. */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < (idx_t)10; ++i)
        res *= num[i];

    return res;
}
