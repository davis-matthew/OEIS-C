/*
 * A018617  Divisors of 735.
 * https://oeis.org/A018617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 735 in increasing order
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Store the divisors of 735 once */
    static seq_t divs[12];
    static size_t divs_count = 0;
    static bool init = false;

    if (!init) {
        /* Enumerate all divisors of 735 in increasing order */
        for (idx_t i = 1; i <= 735; ++i) {
            if ((735 % i) == 0ULL) {
                divs[divs_count++] = (seq_t)i;
            }
        }
        init = true;
    }

    if (n < 1) return (seq_t)0;          /* safety for n < 1 */
    size_t idx = (size_t)(n - 1);        /* convert to zero‑based index */
    if (idx < divs_count) {
        return divs[idx];
    } else {
        return (seq_t)0;                  /* sequence ends after 12 terms */
    }
}
