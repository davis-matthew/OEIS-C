/*
 * A227097  a(n) = n successive applications of the <*>(3,2) operator to 1.
 * https://oeis.org/A227097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n successive applications of the <*>(3,2) operator to 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Apply the <*>(3,2) operator to a single number x.
   From right to left the multipliers alternate 3, 2, 3, 2, ….
   It is equivalent to y = Σ d_i * m_i * 10^i where m_i = 3 if i even, else 2. */
static seq_t apply_operator(seq_t x)
{
    seq_t result = 0;
    seq_t pow10   = 1;
    int mul3      = 1;            /* 1 → use multiplier 3, 0 → use multiplier 2 */

    while (x != 0)
    {
        seq_t digit = x % 10;
        x          = x / 10;
        seq_t mult = mul3 ? 3 : 2;
        result += digit * mult * pow10;
        pow10 *= 10;
        mul3 ^= 1;                /* toggle between 3 and 2 */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t value = 1;
    for (idx_t i = 1; i <= n; ++i)
        value = apply_operator(value);

    return value;
}
