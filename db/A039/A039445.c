/*
 * A039445  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 8's.
 * https://oeis.org/A039445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is computed by brute‑force search from 1 upward, selecting
 *            numbers whose base‑11 representation has the same non‑zero
 *            number of 0’s and 8’s
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t number = 0;
    idx_t found   = 0;

    while (found < n) {
        number = number + 1;                 /* next integer to test */

        uint64_t cnt0 = 0, cnt8 = 0;
        seq_t tmp = number;
        while (tmp > 0) {
            idx_t digit = (idx_t)(tmp % (seq_t)11);
            if (digit == 0)      cnt0++;
            else if (digit == 8) cnt8++;
            tmp = tmp / (seq_t)11;
        }

        if (cnt0 == cnt8 && cnt0 > 0) {
            found++;
            if (found == n)
                return number;
        }
    }
    return 0;    /* unreachable */
}
