/*
 * A160785  Even squarefree numbers plus 1.
 * https://oeis.org/A160785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A039956(n) + 1; where A039956(n) is the nth even
 *            squarefree number
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1; /* start with the smallest odd number */

    while (1) {
        if (is_squarefree(candidate)) {
            count++;
            if (count == n) {
                return candidate * 2 + 1; /* even squarefree + 1 */
            }
        }
        candidate += 2; /* next odd number */
    }
}
