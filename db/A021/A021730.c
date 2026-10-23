/*
 * A021730  Decimal expansion of 1/726.
 * https://oeis.org/A021730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/726 (n=0 is the first digit
 *            after the decimal point). For n > 22, a(n) = a(n‑22)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The first two decimal digits are always 0. */
    if (n == 0 || n == 1)
        return 0;

    /* 22‑digit repeating block of the decimal expansion of 1/726. */
    static seq_t period[22];
    static bool initialized = false;

    if (!initialized) {
        unsigned long long r = 1ULL;          /* initial remainder before first decimal digit */

        /* eat the first two digits (both 0). */
        r = (r * 10ULL) % 726ULL;            /* after a(0) */
        r = (r * 10ULL) % 726ULL;            /* after a(1) */

        /* generate the 22 digits that repeat from a(2) onward. */
        for (idx_t i = 0; i < 22; ++i) {
            r = r * 10ULL;
            period[i] = (seq_t)(r / 726ULL);
            r = r % 726ULL;
        }
        initialized = true;
    }

    /* digits from a(2) onward repeat every 22 positions. */
    return period[(n - 2) % 22];
}
