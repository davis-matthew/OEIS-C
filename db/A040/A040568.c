/*
 * A040568  Continued fraction for sqrt(593).
 * https://oeis.org/A040568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>  /* for size_t */

/* Continued fraction for sqrt(593).  The period after the integer part
   has length 13 and is: [2,1,5,2,2,1,1,2,2,5,1,2,48].
   a(0) is the integer part 24, following terms repeat with this period. */
seq_t a(idx_t n)
{
    static seq_t period[13];
    static idx_t period_len = 0;

    if (period_len == 0) {
        const uint64_t D  = 593ULL;
        const uint64_t a0 = 24ULL;

        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0;
        idx_t i = 0;

        /* Generate the periodic part once. */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[i++] = (seq_t)a;
        } while (a != 2 * a0);  /* 2*a0 is the last term of the period */

        period_len = i;  /* should be 13 */
    }

    if (n == 0) {
        return (seq_t)24;           /* a(0) */
    } else {
        idx_t idx = (n - 1) % period_len;   /* index into period (0‑based) */
        return period[(size_t)idx];
    }
}
