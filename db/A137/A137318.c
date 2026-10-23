/*
 * A137318  Concatenation of segments of the digit sequence 1 3 1 3 1 3 1 3 1
 *          3 1 3 1 3 1 3 1 3....
 * https://oeis.org/A137318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer formed by n digits alternating 1 and 3, starting
 *            with 1 if n*(n-1)/2 is even, otherwise starting with 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* Calculate the parity of the starting position:
       start_pos = T_{n-1} + 1 = n*(n-1)/2 + 1          */
    idx_t tri = n * (n - 1) / 2;          /* triangular number T_{n-1} */
    int start_one = ((tri & 1) == 0);     /* true -> start digit 1, else 3 */

    /* Allocate buffer for the decimal string (+1 for '\0') */
    char *buf = (char *)malloc((size_t)n + 1);
    if (!buf)
        return 0; /* fall back on overflow; seq_t may be 0 */

    /* Build the digit string */
    for (idx_t i = 0; i < n; ++i) {
        char d;
        if (start_one) {
            d = (i & 1) ? '3' : '1';      /* odd index -> 3, even -> 1 */
        } else {
            d = (i & 1) ? '1' : '3';      /* odd index -> 1, even -> 3 */
        }
        buf[i] = d;
    }
    buf[n] = '\0';

    seq_t val = seq_from_str(buf);
    free(buf);
    return val;
}
