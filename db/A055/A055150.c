/*
 * A055150  The first n digits of the juxtaposition of the base 9 numbers
 *          converted to decimal.
 * https://oeis.org/A055150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the first n digits of the infinite
 *            concatenation of the base‑9 representations of 1,2,3,…,
 *            interpreted as a base‑9 number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the types seq_t and idx_t to be defined in the build. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 we return 0 (offset 1; behaviour below offset is
       unspecified but harmless). */
    if (n == 0) return 0;

    seq_t val = 0;          /* running result */
    idx_t  consumed = 0;    /* digits already processed */

    for (idx_t i = 1; consumed < n; ++i) {
        /* Obtain base‑9 digits of i, most significant first. */
        idx_t tmp = i;
        /* The maximum number of base‑9 digits for a 64‑bit i is 20. */
        uint8_t digits[20];
        unsigned dlen = 0;
        do {
            digits[dlen++] = tmp % 9;
            tmp /= 9;
        } while (tmp != 0);

        /* Process the digits in reverse order (most significant first). */
        for (int j = dlen - 1; j >= 0; --j) {
            val = val * 9 + digits[j];
            ++consumed;
            if (consumed == n)
                return val;
        }
    }
    return val;   /* Should never reach here, but keeps compiler happy. */
}
