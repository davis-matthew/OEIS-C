/*
 * A099150  Positive integers k such that f(k)+f(k)=concatenation of k and k,
 *          where f(k)=k(k+3)/2 (A000096).
 * https://oeis.org/A099150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10^n - 2   (offset 1)
 */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) is outside the defined range; return 0 as a safe value. */
    if (n == 0)
        return 0;

    /* Compute 10^n (big‑integer aware) and subtract 2. */
    return pow10(n) - (seq_t)2;
}
