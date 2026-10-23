/*
 * A044968  Numbers whose base-3 representation contains no 0's and exactly
 *          three 1's.
 * https://oeis.org/A044968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 330, u32 n <= 5985, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate numbers whose base‑3 representation has no 0’s and exactly three 1’s.
   a(1) = 13, a(2) = 41, ... as listed in A044968. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for 0, but just return 0 */
        return 0;

    idx_t L = 3;       /* minimal length with 3 digits */
    idx_t cum = 0;     /* count of numbers with length < L */

    /* Find the length L such that total numbers up to length L >= n */
    while (1) {
        /* C(L,3) = L*(L-1)*(L-2)/6 */
        idx_t tri = (L * (L - 1) * (L - 2)) / 6;
        if (cum + tri >= n)
            break;
        cum += tri;
        ++L;
    }

    /* 1‑based index inside numbers of length L */
    idx_t k = n - cum;

    /* Enumerate combinations of positions of the three 1’s in lex order */
    idx_t count = 0;
    for (idx_t i = 0; i <= L - 3; ++i) {
        for (idx_t j = i + 1; j <= L - 2; ++j) {
            for (idx_t m = j + 1; m <= L - 1; ++m) {
                ++count;
                if (count == k) {
                    /* Build the number digit by digit */
                    seq_t val = 0;
                    for (idx_t pos = 0; pos < L; ++pos) {
                        seq_t digit = ((pos == i) || (pos == j) || (pos == m)) ? 1 : 2;
                        val = val * 3 + digit;
                    }
                    return val;
                }
            }
        }
    }
    /* Should never reach here for valid input */
    return 0;
}
