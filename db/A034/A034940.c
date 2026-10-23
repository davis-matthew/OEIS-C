/*
 * A034940  Number of rooted labeled triangular cacti with 2n+1 nodes (n
 *          triangles).
 * https://oeis.org/A034940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = (2n-1)!! * (2n+1)^n (double factorial of odd numbers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"
#ifndef A034940_H
#define A034940_H

/* Helper: compute base^exp for seq_t using exponentiation by squaring. */

/* Public sequence function */
seq_t a(idx_t n) {
    /* Compute (2n-1)!! : product of odd numbers up to 2n-1 */
    seq_t odd = 1;
    for (idx_t j = 0; j < n; ++j) {
        odd *= (seq_t)(2 * j + 1);
    }

    /* Compute (2n+1)^n */
    seq_t base = ((seq_t)n) * 2 + 1;          /* base = 2*n + 1 */
    seq_t pow  = _pow_seq_t(base, n);          /* base**n */

    /* Result = odd * pow */
    odd *= pow;
    return odd;
}

#endif /* A034940_H */
