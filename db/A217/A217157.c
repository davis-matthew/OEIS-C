/*
 * A217157  a(n) is the least value of k such that the decimal expansion of
 *          n^k contains two consecutive identical digits.
 * https://oeis.org/A217157
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive, mul_by_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k such that n^k has two consecutive identical decimal digits
   (computed by successive exponentiation and digit scanning). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_consecutive.h"
#include "../utils/mul_by_small.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* definition applies only for n >= 2 */

    /* decimal digits stored little‑endian (least significant first) */
    uint8_t digits[200] = {0};
    int len = 1;                  /* initialise to 1 (number 1) */
    digits[0] = 1;

    /* Theoretical bound: a(n) <= 16 for n > 2, but we allow a few more steps. */
    const int MAX_K = 32;
    for (int k = 1; k <= MAX_K; ++k) {
        mul_by_small(digits, &len, n);
        if (has_consecutive(digits, len))
            return (seq_t)k;
    }

    /* Should never happen under the conjecture; return 0 as a safe fallback. */
    return 0;
}
