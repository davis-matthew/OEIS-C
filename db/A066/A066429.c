/*
 * A066429  a(n) = 7^n mod n^7.
 * https://oeis.org/A066429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   mod_pow, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* modulus m = n^7 */
    seq_t mod = pow_int((seq_t)n, 7);
    /* compute 7^n mod m */
    return mod_pow((seq_t)7, n, mod);
}
