/*
 * A055931  Product of the digits of n divides the sum of the digits of n.
 * https://oeis.org/A055931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth positive integer whose decimal digit product divides its digit sum. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;        /* number of qualifying integers found so far */
    idx_t num = 0;          /* candidate integer (will start from 1) */

    while (count < n) {
        num++;              /* consider next integer */

        /* digit sum and product calculation */
        idx_t   temp   = num;
        uint64_t sum   = 0;
        uint64_t prod  = 1;
        unsigned  hasZero = 0;

        while (temp > 0) {
            unsigned d = temp % 10;
            sum += d;
            if (d == 0) {
                hasZero = 1;
                break;    /* product would be zero, cannot divide */
            }
            prod *= d;     /* product stays within 64‑bit for any num up to 10^19 */
            temp /= 10;
        }

        if (!hasZero && sum % prod == 0) {
            count++;
            if (count == n) {
                return (seq_t)num;   /* return the nth qualifying integer */
            }
        }
    }

    /* This point should never be reached for valid input (n ≥ 1). */
    return 0;
}
