/*
 * A095601  a(n) = 105 written in base 17 - n.
 * https://oeis.org/A095601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 105 written in base (17 - n), then read as a decimal
 *            integer
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 17 - n is the radix.  For n ≥ 17 the radix is ≤ 0, undefined;
       we simply return 0 in that case. */
    if (n >= 17u)
        return 0;

    /* Radix b = 17 - n.  (b is at least 1 here.) */
    uint64_t b = 17u - n;

    /* Special case: radix 1 (unary).  Result is 10ⁿ⁵⁻¹/9 = a string of 105 ones. */
    if (b == 1u) {
        seq_t result = 0;
        for (idx_t i = 0; i < 105; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Compute the representation of 105 in base b, least‑significant digit first. */
    const uint64_t M = 105u;
    uint64_t tmp = M;
    /* 105 in base 2 needs at most 7 digits; 105 in base 2 gives 7 digits;
       but we use a generous array size. */
    unsigned int digits[16];
    unsigned int cnt = 0;
    while (tmp > 0) {
        digits[cnt++] = (unsigned int)(tmp % b);
        tmp /= b;
    }

    /* Convert that digit sequence to a decimal integer by concatenation.
       For each digit d we multiply the current result by 10^k where k is
       the number of decimal digits of d (1 or 2 here) and add d. */
    seq_t result = 0;
    for (int i = (int)cnt - 1; i >= 0; --i) {
        unsigned int d = digits[i];
        seq_t mult = (d < 10u ? 10u : 100u);  /* d ∈ [0,16] → k∈{1,2} */
        result = result * mult + d;
    }
    return result;
}
