/*
 * A178405  Numbers such that the rounded up arithmetic mean of their digits
 *          differs from their digital root.
 * https://oeis.org/A178405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th integer m ≥ 1 such that * ceil( sum_digits(m) /
 *            digit_count(m) ) != digital_root(m) * Here, sum_digits(m) is the
 *            sum of decimal digits of m, * digit_count(m) is the number of
 *            decimal digits, * and digital_root(m) = 1 + ( (sum_digits(m)-1)
 *            % 9 ) for m > 0. * * The sequence skips an...
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* buffer to hold decimal digits (max 512 digits suffice for 128‑bit */
    uint8_t buf[512];
    size_t len;

    idx_t count = 0;          /* how many qualifying numbers found */
    idx_t i = 1;              /* candidate number as unsigned 64‑bit */

    for (;; ++i) {
        seq_t x = (seq_t)i;   /* convert to seq_t for digit extraction */

        /* Get decimal digits of x */
        get_digits(x, buf, &len);
        unsigned long long sum = 0;     /* sum of digits */
        for (size_t k = 0; k < len; ++k)
            sum += buf[k];

        /* Average (rounded up) */
        unsigned long long avg = (sum + len - 1) / len;

        /* Digital root */
        unsigned long long dr = sum % 9;
        if (dr == 0 && sum > 0)
            dr = 9;

        /* If they differ, we have a valid term */
        if (avg != dr) {
            ++count;
            if (count == n)
                return x;          /* found the n‑th term */
        }
    }
}
