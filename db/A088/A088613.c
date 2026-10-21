/*
 * A088613  Smallest nonsquarefree multiple of n.
 * https://oeis.org/A088613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A088613: Smallest nonsquarefree multiple of n
 *    a(n) = n                   if n is not squarefree
 *    a(n) = 2*n                 if n is even and squarefree
 *    a(n) = 3*n                 if n is odd, squarefree, and divisible by 3
 *    a(n) = 4*n                 if n is odd, squarefree, not divisible by 3
 *
 * Implementation uses the provided is_squarefree() helper.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic and helper checks */
    seq_t sn = (seq_t)n;

    if (!is_squarefree(sn)) {
        /* n itself is already nonsquarefree */
        return sn;
    }

    /* n is squarefree */
    if ((n & 1ULL) == 0) {          /* even */
        return sn * 2;
    } else if (n % 3ULL == 0) {     /* odd and divisible by 3 */
        return sn * 3;
    } else {                        /* odd, not divisible by 2 or 3 */
        return sn * 4;
    }
}
