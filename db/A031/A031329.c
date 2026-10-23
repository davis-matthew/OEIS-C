/*
 * A031329  Position of n-th 4 in A031324.
 * https://oeis.org/A031329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th digit '4' in the infinite decimal
 *            string formed by concatenating the Fibonacci numbers F(2), F(3),
 *            F(4), ... (the second '1' is the first term)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 25, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* for size_t */
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;                /* current position in the concatenated string */
    idx_t count4 = 0;             /* number of '4's seen so far */

    seq_t f_prev = 0;              /* F(0) */
    seq_t f_cur  = 1;              /* F(1) */

    /* buffer large enough for the decimal representation of the largest
       Fibonacci number needed in typical tests (up to ~2000 digits). */
    char buf[8192];
    size_t len;

    while (count4 < n) {
        /* next Fibonacci number: F(k+1) = F(k) + F(k-1) */
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur = f_next;

        len = sizeof(buf);
        seq_to_str(f_next, buf, &len);

        for (size_t i = 0; i < len; ++i) {
            ++pos;                /* advance to next character position */
            if (buf[i] == '4') {
                ++count4;
                if (count4 == n)
                    return pos;   /* exact position of the n-th '4' */
            }
        }
    }

    /* The loop should always return before reaching this point. */
    return pos;
}
