/*
 * A228878  Happy squares: squares whose trajectory under iteration of sum of
 *          squares of digits map includes 1.
 * https://oeis.org/A228878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228878: Happy squares.
   Definition: a(n) is the nth square number such that the number is
   "happy" under iterative sum of squares of digits (base 10).
*/
#include <stdbool.h>

static inline bool is_happy(seq_t n)
{
    if (n == 0) return false;           /* 0 never reaches 1 */
    while (n != 1 && n != 4) {           /* 4 is the entrance to the unhappy cycle */
        seq_t sum = 0;
        seq_t t = n;
        while (t > 0) {
            int d = (int)(t % 10);
            sum += (seq_t)d * (seq_t)d;
            t /= 10;
        }
        n = sum;
    }
    return n == 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 1;
    while (count < n) {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (is_happy(sq)) {
            ++count;
            if (count == n)
                return sq;
        }
        ++i;
    }
    return 0; /* should never be reached */
}
