/*
 * A175594  Numbers having no primitive root.
 * https://oeis.org/A175594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   has_primitive_root_uint64, is_odd_prime_power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth number that has no primitive root.  a(1) = 0.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_primitive_root_uint64.h"
#include "../utils/is_odd_prime_power_uint64.h"

/* OEIS sequence A175594 */
seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 0 */
        return (seq_t)0;

    idx_t target = n - 1;       /* number of values with no primitive root we need */
    uint64_t count = 0;         /* how many such numbers seen so far */
    uint64_t x = 2;             /* start scanning from 2 upwards */

    while (count < target)
    {
        if (!has_primitive_root_uint64(x))
        {
            ++count;
            if (count == target)
                return (seq_t)x;
        }
        ++x;
    }

    /* Should never be reached */
    return (seq_t)0;
}
