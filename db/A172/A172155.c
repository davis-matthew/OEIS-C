/*
 * A172155  a(n) = Omega(6*n-1)*Omega(6*n+1).
 * https://oeis.org/A172155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Omega(6*n-1) * Omega(6*n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

/* Compute the sequence A172155 */
seq_t a(idx_t n) {
    /* For safety we handle n == 0, though the OEIS offset is 1 */
    if (n == 0)
        return 0;

    idx_t v1 = 6 * n - 1;
    idx_t v2 = 6 * n + 1;

    seq_t o1 = bigomega(v1);
    seq_t o2 = bigomega(v2);

    return o1 * o2;
}
