/*
 * A104128  a(n) = p + p^(p+1), where p = prime(n).
 * https://oeis.org/A104128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p + p^(p+1), where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n should be ≥ 1 */
    seq_t p = get_prime((uint64_t)n);           /* p = prime(n) */
    seq_t exp = p + (seq_t)1;                  /* exponent = p + 1 */
    seq_t powp = pow_seq(p, (idx_t)exp);        /* p^(p+1) */
    return p + powp;                           /* p + p^(p+1) */
}
