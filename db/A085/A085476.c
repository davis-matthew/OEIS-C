/*
 * A085476  Periodic Pascal array, read by upward antidiagonals.
 * https://oeis.org/A085476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(i, j mod (i+1)) read by upward antidiagonals of the
 *            periodic Pascal array T(i,j)=C(i, j mod (i+1))
 * Range:     u8 n <= 123, u16 n <= 384, u32 n <= 1338, u64 n <= 4875,
 *            u128 n <= 9869, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Safe binomial coefficient using only integer arithmetic.
   Works for any seq_t that supports +, -, *, /, and the gcd helper. */
static seq_t binom_safe(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t a = (seq_t)(n - i + 1);      /* numerator factor */
        seq_t b = (seq_t)i;                /* denominator factor   */

        /* cancel common factor between a and b */
        seq_t g = gcd(a, b);
        a /= g;
        b /= g;

        /* cancel remaining factors of b with res */
        g = gcd(res, b);
        res /= g;
        b   /= g;                         /* b should now be 1 */

        res *= a;                         /* safe, as res* a == desired coefficient */
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the antidiagonal d containing the index n.  The cumulative
       number of terms up to diagonal d is (d+1)(d+2)/2.  We walk d until
       this value exceeds n. */
    unsigned __int128 cum = 1;   /* cumulative count of terms */
    idx_t d = 0;
    while (cum <= n) {
        ++d;
        cum += (__int128)(d + 1);
    }

    /* Compute the offset of n within its diagonal. */
    idx_t prev_cum = (idx_t)(cum - (d + 1));  /* terms up to previous diagonal */
    idx_t offset   = n - prev_cum;            /* 0 ≤ offset ≤ d */

    /* Coordinates (i, j) in the array T(i,j). */
    idx_t i = d - offset;
    idx_t j = offset;

    /* Apply the periodic Pascal rule. */
    idx_t k = j % (i + 1);

    return binom_safe(i, k);
}
