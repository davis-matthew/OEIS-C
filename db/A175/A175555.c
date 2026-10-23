/*
 * A175555  Preperiodic part of the decimal expansion of 1/k as k runs through
 *          A065502.
 * https://oeis.org/A175555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the preperiodic part of 1/k, where k is
 *            the nth positive integer divisible by 2 or 5
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find k = the nth number divisible by 2 or 5. */
    idx_t low = 1;
    idx_t high = 2 * n;                     /* sufficiently large upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cnt = mid / 2 + mid / 5 - mid / 10; /* count multiples of 2 or 5 <= mid */
        if (cnt >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                          /* k is guaranteed divisible by 2 or 5 */

    /* Count powers of 2 and 5 in k. */
    idx_t a_factor = 0, b_factor = 0;
    idx_t tmp = k;
    while ((tmp & 1) == 0) {                /* divisible by 2 */
        tmp >>= 1;
        ++a_factor;
    }
    while (tmp % 5 == 0) {                  /* divisible by 5 */
        tmp /= 5;
        ++b_factor;
    }

    /* The length of the preperiodic part is max(a_factor, b_factor). */
    idx_t prelen = (a_factor > b_factor) ? a_factor : b_factor;

    /* Compute 10^prelen in seq_t. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < prelen; ++i)
        pow10 *= 10;

    /* The preperiodic part is floor(10^prelen / k). */
    return pow10 / k;
}
