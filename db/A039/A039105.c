/*
 * A039105  Numbers whose base-9 representation has the same number of 4's and
 *          8's.
 * https://oeis.org/A039105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   integers whose base‑9 representation has the same number of 4’s
 *            and 8’s
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool ok(seq_t x)
{
    seq_t c4 = 0, c8 = 0;
    while (x != 0) {
        seq_t d = x % 9;
        if (d == 4)   c4++;
        else if (d == 8) c8++;
        x /= 9;
    }
    return c4 == c8;
}

seq_t a(idx_t n)
{
    idx_t i = 0;
    idx_t cnt = 0;
    while (1) {
        if (ok((seq_t)i)) {
            cnt++;
            if (cnt == n)
                return (seq_t)i;
        }
        i++;
    }
}
