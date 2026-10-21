/*
 * A067477  Smallest n-digit square starting with 7.
 * https://oeis.org/A067477
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ceil(sqrt(7 * 10^(n‑1)))^2 (for n ≥ 3)
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt_seq_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt_seq_custom.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)  /* undefined for n < 3, return 0 */
        return 0;

    /* Compute 10^(n-1) using exponentiation by squaring. */
    seq_t pow10 = 1;
    seq_t base  = 10;
    idx_t exp   = n - 1;
    while (exp) {
        if (exp & 1)
            pow10 *= base;
        base *= base;
        exp >>= 1;
    }

    /* target = 7 * 10^(n-1) */
    seq_t target = pow10 * (seq_t)7;

    /* floor(sqrt(target)) */
    seq_t s = floor_sqrt_seq_custom(target);

    /* If s*s < target, take the next integer for the ceiling */
    if (s * s < target)
        ++s;

    return s * s;                /* ceil(sqrt(target))^2 */
}
