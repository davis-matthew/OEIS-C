/*
 * A028665  Galois numbers for p=3; order of group AGL(n,3).
 * https://oeis.org/A028665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * ∏_{k=0}^{n-1} (3^n − 3^k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow_n = pow3(n);

    /* The product starts with this factor */
    seq_t product = pow_n;
    seq_t cur = (seq_t)1;            /* 3^0 */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term   = pow_n - cur;           /* 3^n – 3^k */
        product = seq_mul(product, term);     /* multiply into product */
        cur = seq_mul(cur, (seq_t)3);         /* next power 3^(k+1) */
    }

    return product;
}
