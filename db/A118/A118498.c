/*
 * A118498  a(n) = 11*n^20 + 11*n^2 + 152821.
 * https://oeis.org/A118498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 11*n^20 + 11*n^2 + 152821
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t power = 1;
    for (idx_t i = 0; i < 20; ++i)
        power *= (seq_t)n;                  /* compute n^20 */

    seq_t result = (seq_t)11 * power;    /* 11 * n^20 */
    result += (seq_t)11 * (seq_t)n * (seq_t)n;  /* + 11 * n^2 */
    result += (seq_t)152821;               /* + 152821 */
    return result;
}
