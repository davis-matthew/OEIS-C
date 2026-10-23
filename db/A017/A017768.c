/*
 * A017768  Binomial coefficients C(52,n).
 * https://oeis.org/A017768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binom(52,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t comb[53];
    static bool initialized = false;

    if (!initialized) {
        comb[0] = (seq_t)1;
        for (idx_t i = 1; i <= 52; ++i) {
            /* Compute comb[i] from comb[i-1] using the identity */
            comb[i] = comb[i - 1] *
                      (seq_t)(52 - i + 1) /
                      (seq_t)i;
        }
        initialized = true;
    }

    if (n > 52)
        return (seq_t)0;

    return comb[n];
}
