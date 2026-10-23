/*
 * A112764  Greatest prime factor of the n-th 5-smooth number.
 * https://oeis.org/A112764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest_prime_factor(A051037(n))
 * Range:     u8 n <= 50, u16 n <= 607, u32 n <= 2783, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   greatest_prime_factor, mul128, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"
#include "../utils/mul128.h"
#include "../utils/greatest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Guard against zero, though n should be ≥1 */
    if (n == 0) return (seq_t)0;

    seq_t *H = seq_malloc(n);
    H[0] = (seq_t)1;            /* first 5‑smooth number */

    idx_t i2 = 0, i3 = 0, i5 = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t v2 = mul128(H[i2], (seq_t)2);
        seq_t v3 = mul128(H[i3], (seq_t)3);
        seq_t v5 = mul128(H[i5], (seq_t)5);

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        H[k] = next;

        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
    }

    return greatest_prime_factor(H[n - 1]);
}
