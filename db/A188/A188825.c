/*
 * A188825  Number of 3Xn binary arrays without the pattern 0 1 diagonally or
 *          antidiagonally.
 * https://oeis.org/A188825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8, n = 1 * a(n) = 16, n = 2 * a(n) = 16 * 3^(n-2), n > 2
 *            * Empirical recurrence: a(n) = 3 * a(n-1) for n > 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Return the nth term of A188825 (offset 1) in seq_t type */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Behaviour below the offset is unspecified; return 0 */
        return 0;
    }
    if (n == 1) {
        return 8;
    }
    if (n == 2) {
        return 16;
    }
    /* For n > 2: a(n) = 16 * 3^(n-2) */
    seq_t pow3 = seq_pow((seq_t)3, n - 2);
    return seq_mul(pow3, (seq_t)16);
}
