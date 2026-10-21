/*
 * A076260  a(n) = 0 if n is a squarefree number, otherwise the distance
 *          between the two nearest squarefree numbers around n:
 *          A067535(n)-A070321(n).
 * https://oeis.org/A076260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is squarefree; otherwise A067535(n)-A070321(n)
 * Range:     u8 n <= 1072, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* seq_t and idx_t are defined by the build */
extern bool is_squarefree(seq_t x);

seq_t a(idx_t n)
{
    /* a(1) is the first term (offset 1) */
    if (is_squarefree((seq_t)n))
        return 0;

    /* Find the nearest squarefree number below n */
    idx_t prev = n - 1;
    while (!is_squarefree((seq_t)prev))
        prev--;

    /* Find the nearest squarefree number above n */
    idx_t next = n + 1;
    while (!is_squarefree((seq_t)next))
        next++;

    /* Return the distance between the two closest squarefree numbers */
    return (seq_t)(next - prev);
}
