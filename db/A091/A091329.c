/*
 * A091329  Bar coding for digits 0 through 9 in Universal Product Code (or
 *          UPC).
 * https://oeis.org/A091329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = UPC right‑side bar code run‑length encoding for digit (n
 *            mod 10). The sequence is periodic with period 10
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[10] = {
        3211, 2221, 2122, 1411, 1132,
        1231, 1114, 1312, 1213, 3112
    };
    return pat[(size_t)(n % 10)];
}
