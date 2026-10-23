/*
 * A173450  Decimal expansion of (69^2)*10^6 + 69^3.
 * https://oeis.org/A173450
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit of (69^2)*10^6 + 69^3 at position n‑9 (n>=10)
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Decimal digits of 4761328509 from most significant to least */
    static const seq_t digits[10] = {4, 7, 6, 1, 3, 2, 8, 5, 0, 9};

    if (n < 10)
        return 0;                     /* indices below the offset are undefined */

    size_t idx = (size_t)(n - 10);
    if (idx >= 10)
        return 0;                     /* indices beyond the 10‑digit number are undefined */

    return digits[idx];
}
