/*
 * A021291  Decimal expansion of 1/287.
 * https://oeis.org/A021291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th digit (0‑indexed) of the decimal expansion of 1/287.
   The period is 30; the digits are computed once by long division and then
   returned modulo 30. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static period array, computed only once */
    static unsigned char period[30];
    static bool initialized = false;
    if (!initialized) {
        idx_t r = (idx_t)1;             /* initial remainder */
        for (idx_t i = 0; i < 30; ++i) {
            r *= (idx_t)10;             /* bring down next digit */
            period[i] = (unsigned char)(r / (idx_t)287);  /* digit */
            r = r % (idx_t)287;          /* new remainder */
        }
        initialized = true;
    }
    idx_t pos = n % (idx_t)30;          /* position in period */
    return (seq_t)period[pos];
}
