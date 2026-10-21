/*
 * A050989  7-idempotent numbers.
 * https://oeis.org/A050989
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   pow_seq_t_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = C(n, 7) * 7^(n-7)
 */

#include <stdint.h>          /* for idx_t and seq_t definitions by the build */
#include "../utils/pow_seq_t_fixed.h"

static seq_t binom7(idx_t n)
{
    /* compute C(n,7) multiplicatively */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= 7; ++i) {
        res = res * ((seq_t)(n - i + 1)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 7) {
        /* values below the offset are undefined for this sequence */
        return (seq_t)0;
    }
    seq_t c = binom7(n);
    seq_t p = pow_seq_t_fixed((seq_t)7, n - 7);
    return c * p;
}
