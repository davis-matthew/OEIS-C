/*
 * A212526  Negative integers in base -4.
 * https://oeis.org/A212526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the base‑-4 representation of -n, interpreted as a
 *            decimal integer
 * Range:     u8 n <= 12, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute digits of -n in base -4.
       We use signed 128‑bit arithmetic to hold the
       intermediate negative value safely for all n that
       fit in idx_t (uint64_t). */
    __int128 x = -(__int128)n;          /* value to convert    */
    uint8_t digits[128];                /* storage for digits   */
    int len = 0;                        /* number of digits    */

    while (x != 0) {
        int r = (int)((x % 4 + 4) % 4);   /* remainder 0..3      */
        digits[len++] = r;
        x = (x - r) / -4;                /* update value        */
    }

    /* Assemble decimal integer from most significant digit. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
