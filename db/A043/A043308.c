/*
 * A043308  a(n)=A033002(n)/5.
 * https://oeis.org/A043308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A033002(n)/5 Here A033002(n) is the nth positive integer
 *            whose hexadecimal representation uses only digits 0,1,2,3 and
 *            has no equal adjacent digits. The desired terms a(n) are that
 *            integer itself divided by 5, which equals the integer because
 *            the hex numbers are already multiples of 5. The...
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint8_t */

seq_t a(idx_t n)
{
    /* 3^k for 0 <= k <= 20 (enough for n up to 10^10) */
    static idx_t pow3[21];
    static int pow3_init = 0;
    if (!pow3_init) {
        pow3[0] = 1;
        for (int i = 1; i < 21; ++i)
            pow3[i] = pow3[i - 1] * (idx_t)3;
        pow3_init = 1;
    }

    /* Find the length L of the hex string representing a(n) */
    idx_t total = 0;
    idx_t L = 0;
    while (1) {
        ++L;
        total += pow3[L];            /* count of length L numbers: 3^L */
        if (total >= n)
            break;
    }

    idx_t prev_total = total - pow3[L];
    idx_t idx0 = n - prev_total - 1; /* 0‑based index within length L block */

    /* Build the hex digits one by one by combinatorial ranking */
    uint8_t digits[25];               /* enough for n up to 10^10 */
    int prev_digit = -1;
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t remaining = L - pos - 1;
        idx_t block = pow3[remaining];

        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0)           /* leading zero not allowed */
                continue;
            if (d == prev_digit)              /* no equal adjacent digits */
                continue;
            if (idx0 < block) {
                digits[pos] = (uint8_t)d;
                prev_digit = d;
                break;                     /* move to next position */
            } else {
                idx0 -= block;                /* skip this block */
            }
        }
    }

    /* Convert hex digits to a decimal seq_t value */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * ((seq_t)16) + (seq_t)digits[i];

    return val;
}
