/*
 * A137719  Sequence based on the pattern [3n, 3n, 3n, 3n+2, 3n+1, 3n+2].
 * https://oeis.org/A137719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137719
 *
 *  Sequence based on the pattern [3n, 3n, 3n, 3n+2, 3n+1, 3n+2]
 *  Offset: 0
 *
 *  Closed‑form:
 *      a(n) = 3 * floor(n / 6) + base[n mod 6]
 *
 *  where   base[0] = 0, base[1] = 2, base[2] = 1,
 *          base[3] = 2, base[4] = 3, base[5] = 3.
 *
 *  This satisfies the recurrence a(n) = 3 + a(n‑6) with
 *  initial values a(0)=0, a(1)=2, a(2)=1, a(3)=2, a(4)=3, a(5)=3.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {0, 2, 1, 2, 3, 3};
    seq_t q = (seq_t)(n / 6);          /* floor(n/6)   */
    seq_t r = (seq_t)(n % 6);          /* n mod 6      */
    return q * (seq_t)3 + base[(size_t)r];
}
