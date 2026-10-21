/*
 * A060343  Smallest prime which is the sum of n composite numbers.
 * https://oeis.org/A060343
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the least prime P such that P ≥ 4·n + 5
 *            (the sum uses one 9 and the rest 4’s, i.e. 9 + 4·(n-1) = 4n+5)
 */
#include <stdbool.h>
/* use the generic prime test for arbitrary precision */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 2 */
    if (n < 2)
        return 0;   /* undefined, but harmless */

    /* start from the smallest possible odd sum of n composites */
    seq_t p = (seq_t)4 * n + 5;   /* 4n + 5 is always odd for n ≥ 2 */

    /* iterate to find the first prime ≥ 4n+5 */
    while (!is_prime_big(p))
        p += 2;   /* keep p odd */

    return p;
}
