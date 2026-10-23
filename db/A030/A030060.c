/*
 * A030060  Third derivative of Catalan generating function/3!.
 * https://oeis.org/A030060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*C(n+3)/6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   catalan, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/seq_gcd.h"

/*
 * Compute the nth term of OEIS sequence A030060.
 * The formula used:
 *   a(n) = (n+1)(n+2)(n+3) * Catalan(n+3) / 6
 *
 * To avoid overflow on 64‑bit builds the denominator 6 is cancelled
 * against the three factors before the final multiplication.
 */
seq_t a(idx_t n) {
    /* Catalan number C(n+3) */
    seq_t cat = catalan(n + 3);

    /* Factors (n+1), (n+2), (n+3) as seq_t */
    seq_t f[3];
    f[0] = (seq_t)(n + 1);
    f[1] = (seq_t)(n + 2);
    f[2] = (seq_t)(n + 3);

    /* Cancel the denominator 6 among the factors */
    seq_t denom = 6;
    for (idx_t i = 0; i < 3 && denom != 1; ++i) {
        seq_t g = seq_gcd(f[i], denom);
        if (g > 1) {
            f[i] /= g;
            denom /= g;
        }
    }
    /* At this point denom must be 1; the product of f[0..2] is
     * (n+1)(n+2)(n+3)/6 expressed in mixed integer form. */

    seq_t result = cat;
    result *= f[0];
    result *= f[1];
    result *= f[2];

    return result;
}
