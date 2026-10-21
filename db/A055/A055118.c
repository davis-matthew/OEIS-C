/*
 * A055118  Base-8 complement of n (write n in base 8, then replace each digit
 *          with its base-8 negative).
 * https://oeis.org/A055118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = the base‑8 complement of n (write n in base 8, replace
 *            each non‑zero digit d by 8−d; zeroes stay zero)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place  = 1;
    idx_t  m = n;        /* working copy of n */
    if (m == 0)
        return 0;         /* a(0) = 0 */

    while (m > 0)
    {
        /* least significant base‑8 digit of the current n */
        uint64_t digit = m % 8ULL;
        uint64_t mapped = (digit == 0ULL ? 0ULL : 8ULL - digit);

        result += (seq_t)mapped * place;

        m /= 8ULL;      /* discard the processed digit */
        if (m > 0)      /* avoid overflow on the last iteration */
            place *= (seq_t)8ULL;
    }
    return result;
}
