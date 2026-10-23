/*
 * A011000  a(n) = binomial coefficient C(n,47).
 * https://oeis.org/A011000
 *
 * Offset:    47    (a(47) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,47)
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 55, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   ugcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ugcd64.h"

seq_t a(idx_t n)
{
    if (n < 47)
        return 0; /* offset undefined */

    const idx_t K = 47;
    idx_t numerators[K];
    idx_t denominators[K];

    /* build numerator and denominator lists */
    for (idx_t i = 0; i < K; ++i) {
        numerators[i] = n - K + 1 + i; /* n - 47 + 1 + i */
        denominators[i] = i + 1;
    }

    /* cancel common factors between numerators and denominators */
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = 0; j < K; ++j) {
            if (denominators[i] == 1)
                break;
            unsigned long long g = ugcd64((unsigned long long)numerators[j],
                                          (unsigned long long)denominators[i]);
            if (g > 1) {
                numerators[j] /= g;
                denominators[i] /= g;
            }
        }
    }

    /* multiply remaining factors */
    seq_t res = 1;
    for (idx_t i = 0; i < K; ++i) {
        res *= (seq_t)numerators[i];
    }

    return res;
}
