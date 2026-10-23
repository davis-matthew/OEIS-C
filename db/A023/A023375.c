/*
 * A023375  a(n+1) = a(n) converted to base 7 from base 4 (written in base
 *          10).
 * https://oeis.org/A023375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) interpreted as a base‑4 number, read as a base‑7 *
 *            number, then evaluated in base‑10. a(1) = 4
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Index n is 1‑based according to the OEIS offset. */
    seq_t val = 4;               /* a(1) */
    if (n <= 1)
        return val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 0;
        seq_t pow  = 1;
        seq_t x    = val;
        while (x > 0) {
            seq_t d = x % 4;
            x      /= 4;
            next += d * pow;
            pow   *= 7;
        }
        val = next;
    }
    return val;
}
