/*
 * A166874  Numbers with vertical symmetry when expressed as Roman numerals.
 * https://oeis.org/A166874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nth number whose Roman numeral uses only vertically symmetric *
 *            characters (I, V, X, M) and is a palindrome. For n > 8 the
 *            sequence * is 1000·(n − 8)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   is_sym_vert_unique
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_sym_vert_unique.h"

seq_t a(idx_t n)
{
    /* Values after the 8th term are simple multiples of 1000. */
    if (n > 8)
        return (seq_t)((n - 8) * 1000);

    /* For n ≤ 8, brute‑force search for the nth valid number. */
    idx_t count = 0;
    idx_t num   = 1;
    while (1) {
        if (is_sym_vert_unique(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
        ++num;
    }
}
