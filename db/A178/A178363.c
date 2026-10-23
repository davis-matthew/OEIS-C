/*
 * A178363  Numbers with rounded up arithmetic mean of digits = 3.
 * https://oeis.org/A178363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest integer x ≥ 1 such that ceil( sum_of_digits(x) /
 *            digit_count(x) ) = 3 i.e. 2 < sum_of_digits(x) / digit_count(x)
 *            ≤ 3 ⇔ 2·d + 1 ≤ S ≤ 3·d where S = sum of digits, d = digit count
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* consecutive search: keep only counter in 64‑bit type */
    uint64_t count = 0;
    uint64_t x = 1;
    while (1) {
        /* compute sum of digits of x */
        uint64_t tmp = x;
        uint64_t sum = 0;
        while (tmp) {
            sum += tmp % 10;
            tmp /= 10;
        }

        /* compute number of digits of x */
        uint64_t d = 0;
        tmp = x;
        do {
            d++;
            tmp /= 10;
        } while (tmp);

        /* test the mean‑condition */
        if (sum >= 2 * d + 1 && sum <= 3 * d) {
            count++;
            if (count == n)
                return (seq_t)x;
        }
        x++;                  /* next candidate */
    }
}
