/*
 * A235997  Decimal expansion of 3*c^2, where c is the speed of light in
 *          vacuum in SI units.
 * https://oeis.org/A235997
 *
 * Offset:    18    (a(18) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) is the n-th decimal digit of 3*299792458^2, counted from
 *            the * most significant digit with offset 18 (i.e. a(18) is the
 *            first digit of the * 18‑digit integer 269626553621045292). For
 *            n<18, the value is 0
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const char digits[] = "269626553621045292";
    const idx_t offset = 18;
    if (n < offset)
        return 0;
    idx_t pos = n - offset;
    if (pos >= sizeof(digits) - 1)   /* after the terminating null */
        return 0;
    return (seq_t)(digits[pos] - '0');
}
