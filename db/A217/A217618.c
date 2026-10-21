/*
 * A217618  a(n) = A217578(n)/n.
 * https://oeis.org/A217618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A217578(n)/n, where A217578(n) is the smallest integer k
 *            > 1 such that all decimal digits of n*k are odd when n is odd,
 *            and all decimal digits are even when n is even. a(n) is that k
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   all_digits_even, all_digits_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/all_digits_even.h"
#include "../utils/all_digits_odd.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* promote n for multiplication */
    if (sn % 2 == 1) {     /* n is odd: look for odd k > 1 */
        for (idx_t k = 3; ; k += 2) {
            seq_t prod = sn * (seq_t)k;
            if (all_digits_odd(prod))
                return (seq_t)k;
        }
    } else {               /* n is even: search all k > 1 */
        for (idx_t k = 2; ; ++k) {
            seq_t prod = sn * (seq_t)k;
            if (all_digits_even(prod))
                return (seq_t)k;
        }
    }
}
