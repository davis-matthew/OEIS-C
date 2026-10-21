/*
 * A230048  Squarefree odious numbers.
 * https://oeis.org/A230048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth squarefree odious number (odious = numbers with an
 *            odd number of 1s in binary, squarefree = no square prime factor)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test parity of 1-bits: true if odd (odious) */
static bool is_odious_uint64(uint64_t x)
{
    unsigned parity = 0;
    while (x) {
        parity ^= 1;
        x &= x - 1;      /* drop lowest 1 bit */
    }
    return parity;
}

/* test if a number is squarefree by trial division */
static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            x /= p;               /* remove first factor */
            if (x % p == 0)       /* second factor found */
                return false;
            while (x % p == 0)    /* remove any further copies */
                x /= p;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for 0, return 0 as safe value */

    idx_t count = 0;
    idx_t candidate = 1;

    while (1) {
        if (is_odious_uint64((uint64_t)candidate) &&
            is_squarefree_uint64((uint64_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
