/*
 * A240849  Quinary happy numbers.
 * https://oeis.org/A240849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   quinary happy numbers – computed by testing each integer
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_quinary_happy, next_base5_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_quinary_happy.h"
#include "../utils/next_base5_square_sum.h"

/* a(n) returns the n-th quinary happy number (offset 1). */
seq_t a(idx_t n)
{
    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (true) {
        if (is_quinary_happy(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
