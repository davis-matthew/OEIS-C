/*
 * A020331  Numbers whose base-3 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*3^floor(log_3(n)+1) + n
 * Range:     u8 n <= 9, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n>=1; for n==0 we give 0 (not used) */
    if (n == 0) return 0;

    /* Count base‑3 digits of n.  This is floor(log_3(n))+1. */
    idx_t t = n;
    idx_t k = 0;
    while (t > 0) {
        t /= 3;
        k++;
    }

    /* Compute 3^k using the provided helper. */
    seq_t pow = pow3(k);

    /* Result: n * 3^k + n.  Cast n to seq_t to keep type uniform. */
    return (seq_t)n * pow + (seq_t)n;
}
