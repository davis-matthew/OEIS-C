/*
 * A230366  a(n) = Sum_{k=1..floor(n/2)} (k^2 mod n).
 * https://oeis.org/A230366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} (k^2 mod n)
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t sum = 0;
    idx_t limit = n >> 1;      /* floor(n/2) */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;          /* compute k^2  */
        seq_t r  = kk % (seq_t)n;                /* k^2 mod n   */
        sum += r;                                /* accumulate   */
    }
    return sum;
}
