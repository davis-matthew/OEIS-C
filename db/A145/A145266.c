/*
 * A145266  A positive integer n is included if there does not exist a
 *          positive integer m such that Sum{k>=0} floor(n/(m+k)) = n.
 * https://oeis.org/A145266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   included
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A145266  
    A positive integer k is in the sequence if there does NOT exist
    a positive integer m such that
        Σ_{t≥0} ⌊k/(m+t)⌋  =  k
    Equivalently, for k the sums S(m) = Σ_{j=m}^{k} ⌊k/j⌋ never equal k
    for any 1 ≤ m ≤ k.  The sequence contains the first such positive
    integers (offset 1).  This implementation checks each integer
    in turn and returns the n‑th one.
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/included.h"

/*  Return the n‑th element of the sequence (1‑based).  The function
    evaluates terms sequentially on demand; it uses no dynamic memory
    and therefore works for both built‑in integer types and any
    arbitrary‑precision integer class that supports the required
    operators. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val   = 0;      /* will start counting from 1 */

    while (found < n) {
        ++val;                   /* test next integer */
        if (included(val))
            ++found;             /* one more term accepted */
    }
    return val;                 /* val is the nth accepted integer */
}
