/*
 * A143071  A positive integer n is included if the number of 1's in the
 *          binary representation of n is a power of 2 (including being
 *          possibly 1).
 * https://oeis.org/A143071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer whose number of 1‑bits in binary
 *            representation is a power of 2
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if v > 0 and v is a power of two. */
static bool is_power_of_two_uint64(uint64_t v)
{
    return v && !(v & (v - 1));
}

/* Count the number of set bits in a 64‑bit integer. */
static unsigned int popcount_u64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;  /* clear the least significant set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t x = 1;  /* start from 1 (offset 1 as required) */

    while (1) {
        unsigned int pop = popcount_u64(x);
        if (is_power_of_two_uint64(pop)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
