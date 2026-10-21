/*
 * A084020  Numbers which are either a divisor or a multiple of their 9's
 *          complement.
 * https://oeis.org/A084020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   * Sequence A084020: numbers m such that * m | (10^k-1 - m) OR *
 *            (10^k-1 - m) | m * where k = number of decimal digits of m. * *
 *            This implementation scans integers m starting from 1, * tests
 *            the above condition and returns the n-th such number. * It uses
 *            the helper functions digit_len and pow10 fr...
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe */

    idx_t count = 0;              /* how many terms have been found so far */
    seq_t m = (seq_t)1;           /* current number to test */

    for (;; ++m) {
        int k = digit_len(m);        /* number of decimal digits of m */

        seq_t base = pow10(k);       /* 10^k */
        base -= (seq_t)1;            /* 10^k - 1 */

        seq_t c = base - m;          /* 9's complement of m */

        /* Condition: m divides c or c divides m (c can be 0) */
        if (c == (seq_t)0 || ((m % c) == (seq_t)0) || ((c % m) == (seq_t)0)) {
            ++count;
            if (count == n)
                return m;           /* n-th term found */
        }
    }
}
