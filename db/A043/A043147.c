/*
 * A043147  Numbers k such that 3 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has35or53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A043147 – numbers k such that the digits 3 and 5 are adjacent in the base‑7
    representation of k (in either order) but not in that of k−1.
    We generate the sequence by a simple scan of all integers.  The
    procedure is O(n) for the n‑th term.  The routine is fully
    memoizable if desired, but a direct search is fast enough for
    the required limits. */

/* include for bool type and literal */
#include <stdbool.h>
#include "../utils/has35or53.h"

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)                       /* offset starts at 1 */
        return 0;

    seq_t count = 0;
    seq_t k = 1;

    for (;; ++k)
    {
        if (has35or53(k) && !has35or53(k - (seq_t)1))
        {
            ++count;
            if (count == n)
                return k;
        }
    }
}
