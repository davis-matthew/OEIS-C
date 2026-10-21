/*
 * A055354  Number of labeled mobiles (circular rooted trees) with n nodes and
 *          7 leaves.
 * https://oeis.org/A055354
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*(n-7) * (63*n^5
 *            - 630*n^4 + 2205*n^3 - 3374*n^2 + 2296*n - 576) / 14631321600
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 8. For n < 8 the sequence is undefined. */
#include <stdint.h>      /* for uint64_t, etc. */
#include "../utils/gcd.h"

static void apply_factor(seq_t *res, seq_t *denom, idx_t val, int mult)
{
    for (int k = 0; k < mult; ++k) {
        seq_t f = (seq_t)val;
        seq_t g = gcd(f, *denom);
        if (g > (seq_t)1) {
            f /= g;
            *denom /= g;
        }
        *res *= f;
    }
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* 1. polynomial part */
    seq_t s = (seq_t)n;
    seq_t poly = 63;
    poly = poly * s - 630;
    poly = poly * s + 2205;
    poly = poly * s - 3374;
    poly = poly * s + 2296;
    poly = poly * s - 576;

    /* 2. denominator */
    seq_t denom = (seq_t)14631321600ULL;   /* 2^14 * 3^6 * 5^2 * 7^2 */

    /* 3. result accumulator */
    seq_t res = poly;

    /* 4. process factorial factors 1..n */
    for (idx_t i = 1; i <= n; ++i)
        apply_factor(&res, &denom, i, 1);

    /* 5. process extra product (n-1)..(n-7) once more */
    for (idx_t i = n - 7; i <= n - 1; ++i)
        apply_factor(&res, &denom, i, 1);

    /* 6. at this point denom should be 1 (otherwise integer division)
       If denom is not 1 we divide the remaining denominator once */
    if (denom > (seq_t)1)
        res /= denom;

    return res;
}
