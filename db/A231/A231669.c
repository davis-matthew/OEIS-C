/*
 * A231669  a(n) = Sum_{i=0..n} digsum_5(i)^2, where digsum_5(i) = A053824(i).
 * https://oeis.org/A231669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digsum_5(i))^2, where digsum_5(i) = sum of
 *            the base‑5 digits of i (A053824)
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   digsum5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digsum5.h"

/* a(n) = Σ_{i=0..n} (digsum_5(i))^2 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t d = digsum5(i);
        sum += d * d;
    }
    return sum;
}
