/*
 * A171492  Numbers that are not divisible by each of their nonzero decimal
 *          digits.
 * https://oeis.org/A171492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _a171492_includes, make_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer that is NOT divisible by
   each of its nonzero decimal digits. This implementation tests each
   integer successively and counts those that satisfy the condition. */
#include <stdbool.h>
#include "../utils/_a171492_includes.h"
#include "../utils/make_seq_t.h"

#ifdef __cplusplus
/* cast a plain integer to the integer‑like sequence type when seq_t is a
   class; works for arbitrary‑precision builds */

#else
/* for builds where seq_t is a built‑in unsigned integer type */

#endif

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based; n == 0 is undefined – return 0. */
    if (n == 0) return (seq_t)0;

    idx_t count   = 0;
    idx_t current = 1;
    while (count < n)
    {
        if (_a171492_includes(current))
        {
            ++count;
            if (count == n)
                return make_seq_t(current);
        }
        ++current;
    }
    /* Unreachable – return 0 to satisfy the compiler. */
    return (seq_t)0;
}
