/*
 * A226282  a(n) = [n/2]!*[(n+1)/2]!*C([n/2],1)*C([(n+1)/2],1).
 * https://oeis.org/A226282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ⌊n/2⌋!*⌈n/2⌉!*⌊n/2⌋*⌈n/2⌉
 * (Here ⌈n/2⌉ = ⌊(n+1)/2⌋)
 */

#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;             // n >= 1 for the defined range

    /* floor(n/2) and floor((n+1)/2) */
    idx_t m = n / 2;
    idx_t k = (n + 1) / 2;

    /* factorials of the two halves */
    seq_t fm = factorial(m);   /* m!  */
    seq_t fk = factorial(k);   /* k!  */

    /* product: m * k * m! * k! */
    seq_t res = (seq_t)m * (seq_t)k * fm * fk;

    return res;
}
