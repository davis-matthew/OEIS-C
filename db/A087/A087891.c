/*
 * A087891  Number of nonsquarefree terms in 3x+1 trajectory started at n.
 * https://oeis.org/A087891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6140, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of nonsquarefree terms in the 3x+1 (Collatz) trajectory of n */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t count = 0;

    while (x > (seq_t)1) {
        if (!is_squarefree(x))
            count += (seq_t)1;

        if ((x % (seq_t)2) == (seq_t)1)  /* odd */
            x = (seq_t)3 * x + (seq_t)1; /* 3x + 1 */
        else
            x = x / (seq_t)2;           /* x / 2 */
    }

    return count;
}
