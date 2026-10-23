/*
 * A101412  Least number of odd squares that sum to n.
 * https://oeis.org/A101412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Least number of odd squares that sum to n, defined by case
 *            analysis on n mod 8 (OEIS A101412)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   has_two_odd_square_sum, int_sqrt_idx, is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_square.h"
#include "../utils/has_two_odd_square_sum.h"
#include "../utils/int_sqrt_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index 0 unused */

    idx_t mod8 = n & 7;                 /* n % 8 */

    /* n ≡ 1 (mod 8) */
    if (mod8 == 1) {
        return (seq_t)(is_square((seq_t)n) ? 1 : 9);
    }

    /* n ≡ 2 (mod 8) */
    if (mod8 == 2) {
        if (n % 72 == 42 || n % 72 == 66)
            return (seq_t)10;
        return (seq_t)(has_two_odd_square_sum((seq_t)n) ? 2 : 10);
    }

    /* Remaining residues */
    switch (mod8) {
        case 3: return (seq_t)3;
        case 4: return (seq_t)4;
        case 5: return (seq_t)5;
        case 6: return (seq_t)6;
        case 7: return (seq_t)7;
        case 0: return (seq_t)8;
        default: return (seq_t)0;     /* unreachable */
    }
}
