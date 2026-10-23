/*
 * A100909  Concatenate number of occurrences in n of each decimal digit from
 *          0 to 9 and drop leading zeros.
 * https://oeis.org/A100909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate the decimal counts of digits 0–9 in n, then
 *            drop leading zeros
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* count occurrences of each decimal digit */
    uint16_t cnt[10] = {0};
    idx_t tmp = n;

    if (tmp == 0) {
        cnt[0] = 1;               /* n = 0 has one zero digit */
    } else {
        while (tmp) {
            cnt[tmp % 10]++;       /* increment digit count */
            tmp /= 10;
        }
    }

    /* build the answer, dropping leading zeros */
    bool started = false;          /* true once first non‑zero digit is seen */
    seq_t result = 0;

    for (int d = 0; d < 10; ++d) {
        uint16_t c = cnt[d];

        if (c == 0) {              /* single '0' digit */
            char ch = '0';
            if (!started) {
                if (ch != '0') {
                    started = true;
                    result = result * 10 + (seq_t)(ch - '0');
                }
            } else {
                result = result * 10 + (seq_t)(ch - '0');
            }
        } else {                  /* convert c to its decimal digits */
            char buf[6];          /* c <= 20 for any 64‑bit n, so 6 is safe */
            int pos = 0;
            uint16_t x = c;
            while (x > 0) {       /* build reverse order */
                buf[pos++] = '0' + (x % 10);
                x /= 10;
            }
            for (int i = pos - 1; i >= 0; --i) {
                char ch = buf[i];
                if (!started) {
                    if (ch != '0') {
                        started = true;
                        result = result * 10 + (seq_t)(ch - '0');
                    }
                } else {
                    result = result * 10 + (seq_t)(ch - '0');
                }
            }
        }
    }

    return result;
}
