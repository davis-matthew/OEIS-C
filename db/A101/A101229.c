/*
 * A101229  Perfect inverse "3x+1 conjecture" (See comments for rules).
 * https://oeis.org/A101229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101229 – Perfect inverse "3x+1 conjecture" */

/*   a(1)  = 1
    a(2)  = 2
    a(3)  = 4
    a(4)  = 1
    a(5)  = 2
    a(6)  = 4
    a(7)  = 8
    a(8)  = 16
    a(9)  = 5
    a(10) = 10
    for n >= 11:
    a(n) = 3 * 2^(n-11)            */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Terms 1..10: hard‑coded by their own definition. */
    if (n <= 10)
    {
        static const seq_t init[10] =
        {
            1,  /* a(1) */
            2,  /* a(2) */
            4,  /* a(3) */
            1,  /* a(4) */
            2,  /* a(5) */
            4,  /* a(6) */
            8,  /* a(7) */
            16, /* a(8) */
            5,  /* a(9) */
            10  /* a(10) */
        };
        return init[n-1];
    }

    /* a(n) = 3 * 2^(n-11) for n >= 11 */
    seq_t result = 3;
    idx_t shift = n - 11;              /* number of times to double */
    while (shift-- > 0)
        result <<= 1;                  /* shift left = multiply by 2 */
    return result;
}
