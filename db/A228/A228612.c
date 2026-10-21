/*
 * A228612  Number of (possibly overlapping) occurrences of the subword given
 *          by the binary expansion of n in all binary words of length n.
 * https://oeis.org/A228612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - L + 1) * 2^(n - L), where L = ⌊log2 n⌋ + 1 (number
 *            of bits in n). a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute L = number of bits required to represent n (i.e. floor(log2(n)) + 1). */
    idx_t L = 0;
    idx_t temp = n;
    while (temp > 0) {
        temp >>= 1;
        ++L;
    }

    /* Exponent for 2: n - L. */
    idx_t exp = n - L;

    /* Compute 2^exp by repeated multiplication.  This works for any seq_t type
       (built‑in or arbitrary‑precision) as long as operator* is defined. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= 2;

    /* Final result: (n - L + 1) * 2^(n - L). */
    seq_t result = (seq_t)(n - L + 1) * pow2;
    return result;
}
