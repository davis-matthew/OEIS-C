/*
 * A233398  n^n mod 3^n.
 * https://oeis.org/A233398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n mod 3^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   mod_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0; a(n) = 0 when n is a multiple of 3 */
#include "../utils/pow_seq.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n % 3 == 0) return 0;

    seq_t mod = pow_seq((seq_t)3, (int)n);
    return mod_pow((seq_t)n, n, mod);
}
