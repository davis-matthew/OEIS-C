/*
 * A061452  n*bigomega(n)^n, where bigomega(n) is the number of prime divisors
 *          of n, counted with multiplicity.
 * https://oeis.org/A061452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n*bigomega(n)^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* n >= 1 according to OEIS offset */
    seq_t n_val = (seq_t)n;
    seq_t w = bigomega(n);          /* bigomega(n) */
    seq_t pw = big_pow(w, n_val);   /* w^n */
    return pw * n_val;              /* n * w^n */
}
