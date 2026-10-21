/*
 * A072896  5th-order digital invariants: the sum of the 5th powers of the
 *          digits of n equals some number k and the sum of the 5th powers of
 *          the digits of k equals n.
 * https://oeis.org/A072896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = numbers n such that the sum of the fifth powers of the
 *            digits of n equals k and the sum of the fifth powers of the
 *            digits of k equals n. The sequence consists of all such n,
 *            sorted ascending, starting with a(1)=1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   sum5_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sum5_digits.h"

static seq_t seq[400000];
static size_t seq_len = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        const idx_t MAX_N = 354294;   /* upper bound for possible solutions */
        for (idx_t i = 1; i <= MAX_N; ++i) {
            uint64_t s1 = sum5_digits((uint64_t)i);
            uint64_t s2 = sum5_digits(s1);
            if ((idx_t)s2 == i) {
                seq[seq_len++] = (seq_t)i;
            }
        }
        initialized = 1;
    }
    if (n < 1 || (size_t)n > seq_len) return 0;   /* out of range */
    return seq[n - 1];
}
