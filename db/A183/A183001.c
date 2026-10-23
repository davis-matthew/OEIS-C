/*
 * A183001  Decimal expansion of the integer c^4 where c = 299792458 (exactly)
 *          is the speed of light in vacuum (m/s).
 * https://oeis.org/A183001
 *
 * Offset:    34    (a(34) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = digit of 299792458^4 at position (n-34) for n≥34, 0 for
 *            n<34 or n beyond the length of the decimal expansion
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const char *const DIGITS =
    "8077608713062490229263800746151696";

seq_t a(idx_t n)
{
    /* Offsets: a(34) is first digit. */
    if (n < 34)
        return 0;               /* below the offset */

    idx_t pos = n - 34;         /* 0‑based position */
    if (pos < 34)
        return (seq_t)(DIGITS[pos] - '0'); /* digit value 0‑9 */

    return 0;                   /* beyond the number – trailing zeros */
}
