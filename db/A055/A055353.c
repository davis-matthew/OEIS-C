/*
 * A055353  Number of labeled mobiles (circular rooted trees) with n nodes and
 *          6 leaves.
 * https://oeis.org/A055353
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)^2 * (n-2)^2 * (n-3) * (n-4) * (n-5) * (n-6) *
 *            (3*n^2-13*n+8) / 8294400 , n >= 7
 * Range:     u8 none, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   cancel_from_result
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cancel_from_result.h"
/* Helper to divide out prime factors of 2, 3 and 5 from a number,
   decreasing the remaining exponent counters.                */
static seq_t reduce_factor(seq_t f, unsigned *e2, unsigned *e3, unsigned *e5)
{
    while (*e2 > 0 && f % 2 == 0) { f /= 2; (*e2)--; }
    while (*e3 > 0 && f % 3 == 0) { f /= 3; (*e3)--; }
    while (*e5 > 0 && f % 5 == 0) { f /= 5; (*e5)--; }
    return f;
}

seq_t a(idx_t n)
{
    if (n < 7) return 0;   /* values below the offset are irrelevant */

    seq_t res = 1;
    unsigned e2 = 12, e3 = 4, e5 = 2;   /* exponents of 2,3,5 in the denominator */

    /* factorial n! */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t f = (seq_t)i;
        f = reduce_factor(f, &e2, &e3, &e5);      /* remove any denom factors from the factor */
        res *= f;
        cancel_from_result(&res, &e2, &e3, &e5);   /* cancel if possible */
    }

    /* (n-1)^2 */
    seq_t nm1 = (seq_t)(n - 1);
    res *= nm1;
    cancel_from_result(&res, &e2, &e3, &e5);
    res *= nm1;
    cancel_from_result(&res, &e2, &e3, &e5);

    /* (n-2)^2 */
    seq_t nm2 = (seq_t)(n - 2);
    res *= nm2;
    cancel_from_result(&res, &e2, &e3, &e5);
    res *= nm2;
    cancel_from_result(&res, &e2, &e3, &e5);

    /* (n-3), (n-4), (n-5), (n-6) */
    res *= (seq_t)(n - 3);
    cancel_from_result(&res, &e2, &e3, &e5);
    res *= (seq_t)(n - 4);
    cancel_from_result(&res, &e2, &e3, &e5);
    res *= (seq_t)(n - 5);
    cancel_from_result(&res, &e2, &e3, &e5);
    res *= (seq_t)(n - 6);
    cancel_from_result(&res, &e2, &e3, &e5);

    /* (3*n^2 - 13*n + 8) */
    /* compute safely avoiding overflow of 64‑bit intermediate */
    seq_t big = (seq_t)(((__int128)3 * n * n) - (__int128)13 * n + 8);
    res *= big;
    cancel_from_result(&res, &e2, &e3, &e5);

    /* All denominator factors should be cancelled now */
    /* If any remain, divide them explicitly (should not happen). */
    while (e2 > 0) { res /= 2; --e2; }
    while (e3 > 0) { res /= 3; --e3; }
    while (e5 > 0) { res /= 5; --e5; }

    return res;
}
