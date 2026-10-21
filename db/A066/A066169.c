/*
 * A066169  Least k such that phi(k) >= n.
 * https://oeis.org/A066169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k such that φ(k) ≥ n; computed by incremental
 *            search
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    idx_t k = n;
    while (true) {
        if (phi(k) >= (seq_t)n) {
            break;
        }
        ++k;
    }
    return (seq_t)k;
}
