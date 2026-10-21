/*
 * A061818  Multiples of 2 containing only digits 0,1,2.
 * https://oeis.org/A061818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this sequence lists all decimal numbers composed of the digits
 *            0,1,2 that are even (last digit 0 or 2); the first term is 0
 *            (offset 0). The nth term (0‑based) is given by a mixed radix
 *            construction: first digit 1 or 2, middle digits 0–2, last digit
 *            0 or 2
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   build_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_number.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    /* Handle numbers > 0.  Subtract the zero term from the index. */
    idx_t remaining = n - 1;              /* zero‑based among positive terms */

    /* Numbers with a single non‑zero digit are just "2". */
    if (remaining < 1) return (seq_t)2;    /* a(1) = 2 */
    remaining -= 1;

    /* Iterate over increasing digit lengths. */
    idx_t pow3 = 1;                       /* 3^(len-2);  for len=2 it's 3^0=1. */
    for (uint8_t len = 2; ; ++len) {
        idx_t count_len = 4 * pow3;       /* count of len‑digit even numbers */

        if (remaining < count_len) {
            /* The desired number lies in this block. */
            return build_number(remaining, len);
        }

        remaining -= count_len;           /* skip this block */
        pow3 *= 3;                        /* prepare for next length */
    }
}
