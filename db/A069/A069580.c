/*
 * A069580  Number of noninvertible n X n matrices mod 4 (i.e., over the ring
 *          Z_4).
 * https://oeis.org/A069580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n^2) − 2^(n^2) * Π_{i=0}^{n-1} (2^n – 2^i)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Number of non‑invertible n×n matrices over Z₄) */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is defined for n ≥ 1; return 0 for n = 0 */
        return (seq_t)0;
    }

    /* total number of n×n matrices over Z₄: 4^(n²) = 2^(2n²) */
    seq_t total = pow2(2 * n * n);

    /* 2^n as a big integer */
    seq_t pow2_n = pow2(n);

    /* product ∏_{i=0}^{n-1} (2^n – 2^i) */
    seq_t prod = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = pow2_n - pow2(i);
        prod *= term;
    }

    /* number of invertible matrices over Z₄ */
    seq_t invertible = pow2(n * n) * prod;

    /* non‑invertible matrices */
    return total - invertible;
}
