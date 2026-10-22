/*
 * A003031  Denominators of expansion of Fresnel integral S(z).
 * https://oeis.org/A003031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+3) * 2^(2*n+1) * (2*n+1)!
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n)
{
	seq_t factorial = 1;
	for (idx_t i = 2; i <= 2 * n + 1; i++) {
		factorial = factorial * (seq_t)i;
	}
	
	seq_t result = ((seq_t)(4 * n + 3)) * factorial * A001832_pow2(2 * n + 1);
	return result;
}
