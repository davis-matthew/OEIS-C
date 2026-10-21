/*
 * A066559  a(n) = ceiling(10^(n-1)/n).
 * https://oeis.org/A066559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(10^(n-1)/n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)1;
    }

    /* k = n-1 : number of zero digits after the leading 1 */
    idx_t k   = n - 1u;
    idx_t rem = 1u % n;        /* remainder after the leading digit 1 */

    /* Buffer for the decimal representation of the quotient */
    static char qbuf[20005];
    size_t pos = 0;

    idx_t qdigit;
    /* Process each subsequent zero digit */
    for (idx_t i = 0u; i < k; ++i) {
        rem = rem * 10u;              /* bring next zero into the remainder */
        qdigit = rem / n;             /* quotient digit (0‑9) */
        if (pos > 0u || qdigit > 0u) {/* skip leading zeros */
            qbuf[pos++] = (char)('0' + qdigit);
        }
        rem %= n;
    }

    /* If the quotient is zero, store a single '0' */
    if (pos == 0u) {
        qbuf[pos++] = '0';
    }

    /* If there was a remainder, the ceiling requires adding one */
    if (rem > 0u) {
        /* Increment the decimal string by 1 */
        size_t i = pos;
        while (i > 0u) {
            int d = (qbuf[i - 1] - '0') + 1;
            if (d == 10) {
                qbuf[i - 1] = '0';
                --i;
            } else {
                qbuf[i - 1] = (char)('0' + d);
                break;
            }
        }
        if (i == 0u) {            /* carry propagated past the most significant digit */
            /* shift all digits one position to the right */
            for (size_t j = pos; j > 0u; --j) {
                qbuf[j] = qbuf[j - 1];
            }
            qbuf[0] = '1';
            ++pos;
        }
    }

    qbuf[pos] = '\0';               /* null‑terminate the string */
    return seq_from_str(qbuf);
}
