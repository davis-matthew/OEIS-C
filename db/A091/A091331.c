/*
 * A091331  Natural numbers written out with their digits grouped in triples
 *          (leading zeros omitted).
 * https://oeis.org/A091331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth triple of the concatenated natural numbers, with
 *            leading zeros omitted
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* buffer for up to three digits of the current triple */
    int buf[3];
    int buf_len = 0;
    seq_t result = 0;
    seq_t groups = 0;          /* number of triples produced so far */
    idx_t cur = 1;             /* current natural number being processed */
    int digits[20];            /* temporary storage of digits of cur (max 20 for uint64) */

    while (1) {
        /* Extract digits of cur into 'digits' in reverse order */
        idx_t tmp = cur;
        int dlen = 0;
        do {
            digits[dlen++] = (int)(tmp % 10);
            tmp /= 10;
        } while (tmp > 0);

        /* Process digits in correct order */
        for (int i = dlen - 1; i >= 0; --i) {
            buf[buf_len++] = digits[i];
            if (buf_len == 3) {
                /* Convert buffered digits to seq_t, dropping leading zeros */
                seq_t val = 0;
                int leading = 1;
                for (int j = 0; j < 3; ++j) {
                    int d = buf[j];
                    if (leading && d == 0) {
                        continue;
                    }
                    leading = 0;
                    val = val * 10 + d;
                }
                /* If all digits were zero (unlikely), keep val = 0 */
                groups++;
                if (groups == n) {
                    return val;
                }
                buf_len = 0;   /* reset buffer for next triple */
            }
        }

        /* Advance to next natural number */
        cur++;
    }
}
