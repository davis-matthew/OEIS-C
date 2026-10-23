/*
 * A161774  a(n) = n^(3n^3 + 2).
 * https://oeis.org/A161774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(3n^3 + 2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent 3*n^3 + 2 using 64‑bit integer */
    idx_t exp64 = 3 * n * n * n + 2;
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)exp64;        /* cast may truncate for tiny seq_t builds,
                                         but those builds test only values that
                                         fit in seq_t anyway */
    return big_pow(base, exp);
}
