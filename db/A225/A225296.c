/*
 * A225296  Numbers divisible by their first digit cubed (excluding those
 *          whose first digit is 1).
 * https://oeis.org/A225296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Compute A225296: numbers divisible by their first digit cubed,
    excluding numbers whose first digit is 1.
    a(n) returns the n-th such number for n >= 1. */
static inline int first_digit_uint64(uint64_t x)
{
    while (x >= 10)
        x /= 10;
    return (int)x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, safe guard */

    uint64_t found = 0;
    uint64_t candidate = 1;

    while (1) {
        int fd = first_digit_uint64(candidate);
        if (fd != 1) {
            uint64_t cube = (uint64_t)fd * fd * fd;
            if (candidate % cube == 0) {
                ++found;
                if (found == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
