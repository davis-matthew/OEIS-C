/*
 * A050188  T(n,3), array T as in A050186; a count of aperiodic binary words.
 * https://oeis.org/A050188
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,3) - (n/3 if n ≡ 0 (mod 3) else 0)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(3) = 0, a(n) follows the pattern shown above. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute C(n,3) */
    seq_t val = binom(n, 3);

    /* Subtract n/3 when n is divisible by 3 */
    if ((n % 3) == 0) {
        seq_t sub = (seq_t)(n / 3);
        val -= sub;
    }

    return val;
}
