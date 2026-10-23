/*
 * A169923  Integer values of 3*n-36+360/(n+10) (see A169922).
 * https://oeis.org/A169923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 3*d - 66 + 360/d, where d is the n‑th divisor of 360 in
 *            ascending order that is ≥ 10. The sequence is finite; terms are
 *            returned for 1 ≤ n ≤ 16. Out‑of‑range indices yield 0
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for positive indices */
    if (n < 1)
        return 0;

    /* static cache of the 16 relevant divisors of 360 (ascending) */
    static idx_t divs[16];
    static size_t divcnt = 0;
    static bool init = false;
    if (!init) {
        size_t k = 0;
        for (idx_t i = 10; i <= 360; ++i) {
            if (360 % i == 0) {
                divs[k++] = i;
            }
        }
        divcnt = k;           /* should be 16 for 360 */
        init = true;
    }

    if (n > divcnt)
        return 0;

    idx_t d = divs[n - 1];
    /* compute value using 64‑bit arithmetic to avoid overflow, then
       cast to the build‑specific seq_t type. */
    idx_t val = (idx_t)3 * d - 66 + (idx_t)360 / d;
    return (seq_t)val;
}
