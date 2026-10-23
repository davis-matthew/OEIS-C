/*
 * A121382  Number of ways of writing n as x*y*z, with x <= y <= z and
 *          gcd(x,y) = gcd(y,z) = 1.
 * https://oeis.org/A121382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4864, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n as x*y*z with x <= y <= z,
   gcd(x,y) = gcd(y,z) = 1 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Collect all divisors of n into a small stack array. */
    static const size_t MAXDIV = 256;
    idx_t divs[MAXDIV];
    size_t divc = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[divc++] = i;
            if (i != n / i && divc < MAXDIV)
                divs[divc++] = n / i;
        }
    }

    seq_t count = 0;

    /* Iterate over y, then candidate x divisors of n. */
    for (size_t yi = 0; yi < divc; ++yi) {
        idx_t y = divs[yi];
        idx_t m = n / y;               /* Remaining factor for x*z */

        for (size_t xi = 0; xi < divc; ++xi) {
            idx_t x = divs[xi];
            if (x > y) continue;          /* enforce x <= y */
            if (m % x != 0) continue;      /* x must divide m */

            idx_t z = m / x;
            if (y > z) continue;          /* enforce y <= z */

            /* Check coprimality of adjacent factors. */
            if (gcd((seq_t)x, (seq_t)y) != 1) continue;
            if (gcd((seq_t)y, (seq_t)z) != 1) continue;

            /* Caught one valid factorization. */
            ++count;
        }
    }

    return count;
}
