/*
 * A124810  Number of 4-ary Lyndon words of length n with exactly two 1s.
 * https://oeis.org/A124810
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2 * (n-1) * 3^(n-2) if n is odd a(n) = 1/2 * (n-1) *
 *            3^(n-2) - 1/2 * 3^((n-2)/2) if n is even (for n >= 3, a(3)=3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Return 0 for indices below the offset; sequence is defined only for n >= 3. */
    if (n < 3) return 0;

    /* 3^(n-2) */
    seq_t pow_term = pow3((idx_t)(n - 2));

    /* (n-1) * 3^(n-2) / 2  */
    seq_t result = ((seq_t)(n - 1)) * pow_term;
    result = result / 2;

    /* If n is even, subtract the extra term: 3^((n-2)/2) / 2  */
    if ((n & 1ULL) == 0) {
        seq_t sub = pow3((idx_t)((n - 2) / 2));
        sub = sub / 2;
        result = result - sub;
    }

    return result;
}
