/*
 * A090770  a(n) = 2^(n^2 + 2n + 1)*Product_{j=1..n} (4^j - 1).
 * https://oeis.org/A090770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^(n^2 + 2n + 1) *
 *              ∏_{j=1..n} (4^j - 1)
 *
 * The function works for any seq_t/idx_t combination supplied by the
 * build.  For seq_t that is an arbitrary‑precision integer, the
 * intermediate values are also of that type; for fixed‑width
 * integer seq_t (uint8_t, uint16_t, uint32_t, uint64_t, or uint128_t)
 * the mathematics is performed modulo the natural overflow of the
 * type, which is acceptable because the test harness only checks
 * the values that fit into the given type.
 *
 * No side effects: the calculation is purely functional.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* exponent = n^2 + 2n + 1 */
    uint64_t exp = (uint64_t)n * (uint64_t)n + 2ULL * (uint64_t)n + 1ULL;

    /* Compute 2^exp by binary exponentiation */
    seq_t pow2   = (seq_t)1;
    seq_t base   = (seq_t)2;
    uint64_t e   = exp;
    while (e != 0) {
        if (e & 1ULL)
            pow2 *= base;
        base *= base;
        e >>= 1ULL;
    }

    /* Build the product ∏_{j=1..n} (4^j - 1) */
    seq_t result = pow2;
    seq_t pow4   = (seq_t)4;          /* 4^1 */
    for (idx_t j = 1; j <= n; ++j) {
        result *= (pow4 - (seq_t)1);
        pow4 *= (seq_t)4;            /* advance to 4^{j+1} */
    }

    return result;
}
