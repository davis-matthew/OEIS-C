/*
 * A129106  Decimal expansion of proposed value for Avogadro's number, namely
 *          602214141070409084099072 = 84446888^3.
 * https://oeis.org/A129106
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit at position n of 602214141070409084099072 * (offset
 *            24 – a(24) is the leading 6)
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Decimal digits of 602214141070409084099072. */
    static const char digits[] = "602214141070409084099072";
    static const size_t len = sizeof(digits) - 1;    /* 24 */

    if (n < (idx_t)24 || n >= (idx_t)(24 + len)) {
        return (seq_t)0;   /* undefined for n outside the range */
    }

    return (seq_t)(digits[n - 24] - '0');
}
