/*
 * A055948  n + reversal of base 4 digits of n (written in base 10).
 * https://oeis.org/A055948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + R(n)
 *
 * R(n) is the value obtained by reversing the base‑4 representation of n
 * and interpreting the reversed digits again as a base‑4 number.
 * For example:
 *   n = 4  (base‑4 "10")  →  reversed "01" → decimal 1
 *   a(4) = 4 + 1 = 5
 */

#include <stdbool.h>    /* for bool, not otherwise used */
#include "../utils/reverse_base4.h"

seq_t a(idx_t n)
{
    const seq_t rev = reverse_base4(n);
    return (seq_t)n + rev;
}
