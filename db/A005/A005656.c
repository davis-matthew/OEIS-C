/*
 * A005656  Number of bracelets (turn over necklaces) with n red, 1 pink and n
 *          - 3 blue beads; also reversible strings with n red and n-3 blue
 *          beads.
 * https://oeis.org/A005656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(binomial(2*n - 3, n - 3) + binomial(n - 2,
 *            floor((n - 3)/2)))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a5656_binom, a5656_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a5656_binom.h"
#include "../utils/a5656_gcd.h"

seq_t a(idx_t n)
{
    seq_t xp, b2, h1, h2;
    idx_t q;
    bool odd1, odd2;

    if (n < 3) return 0;

    if (n == 3) {
        xp = 1;
        q = 1;
    } else {
        /* binomial(2n-3, n-3) = binomial(2n-4, n-4) * (2n-3) / (n-3) */
        seq_t x = a5656_binom(2 * n - 4, n - 4);
        seq_t g = a5656_gcd(x, (seq_t)(n - 3));
        idx_t gi = (idx_t)(long long)g;
        idx_t d = (n - 3) / gi;
        xp = x / g;
        q = (2 * n - 3) / d;
    }
    /* b1 = xp * q; compute floor(b1/2) without forming b1 */
    if (q % 2 == 0) {
        h1 = xp * (seq_t)(q / 2);
        odd1 = false;
    } else if (xp % 2 == 0) {
        h1 = (xp / 2) * (seq_t)q;
        odd1 = false;
    } else {
        h1 = xp * (seq_t)((q - 1) / 2) + (xp - 1) / 2;
        odd1 = true;
    }

    b2 = a5656_binom(n - 2, (n - 3) / 2);
    h2 = b2 / 2;
    odd2 = (b2 % 2) != 0;

    return h1 + h2 + ((odd1 && odd2) ? 1 : 0);
}
