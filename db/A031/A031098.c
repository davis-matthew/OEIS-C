/*
 * A031098  Write n in base 9, then complement each digit (d -> 8-d) and
 *          juxtapose.
 * https://oeis.org/A031098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth digit in the concatenated string obtained by
 *            writing the positive integers 1,2,3,… in base 9, replacing each
 *            base-9 digit d by its complement 8−d, and juxtaposing the
 *            resulting digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>     /* for NULL, size_t etc. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, return 0 */

    idx_t total = 0;                     /* total digits seen so far */
    idx_t m = 1;                         /* current integer being processed */

    /* Find the integer m such that the nth digit lies in its complement. */
    while (1) {
        idx_t temp = m;
        idx_t len = 0;                   /* number of base‑9 digits of m */
        do {
            temp /= 9;
            ++len;
        } while (temp > 0);

        if (total + len >= n)            /* digit is inside this number */
            break;

        total += len;
        ++m;
    }

    /* Position of the desired digit within the complement of m (0‑based). */
    idx_t offset = n - total - 1;         

    /* Extract the base‑9 digits of m (least significant first). */
    idx_t temp = m;
    uint8_t digs[32];                     /* enough for 64‑bit numbers */
    idx_t dlen = 0;
    do {
        digs[dlen++] = (uint8_t)(temp % 9);
        temp /= 9;
    } while (temp > 0);

    /* Most significant digit index */
    idx_t ms_index = dlen - 1 - offset;
    uint8_t orig_digit = digs[ms_index];
    uint8_t comp_digit = 8 - orig_digit;

    return (seq_t)comp_digit;
}
