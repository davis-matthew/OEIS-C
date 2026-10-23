/*
 * A171671  Square numbers not of form m + sum of digits of m.
 * https://oeis.org/A171671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 7143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: squares not of the form m + digit_sum(m) (Kaprekar self‑squares) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the sum of decimal digits of a 64‑bit unsigned integer. */
static inline uint64_t digit_sum_u64(uint64_t x)
{
    uint64_t sum = 0;
    do {
        sum += x % 10;
        x /= 10;
    } while (x);
    return sum;
}

/* Count the number of decimal digits of a 64‑bit unsigned integer. */
static inline unsigned digits_u64(uint64_t x)
{
    unsigned d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Return true iff the square S can be expressed as m + digit_sum(m)
   for some integer m.  S is assumed to fit into 64 bits. */
static inline bool square_is_representable(seq_t S)
{
    uint64_t s = (uint64_t)S;
    unsigned dmax = 9 * digits_u64(s);
    for (unsigned d = 0; d <= dmax; ++d) {
        uint64_t m = s - d;
        if (digit_sum_u64(m) == d)
            return true;
    }
    return false;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by problem statement. */
    idx_t count = 0;
    idx_t r = 1;  /* root of the square */
    for (;;) {
        seq_t sq = (seq_t)r * (seq_t)r;  /* r² */
        if (!square_is_representable(sq)) {
            ++count;
            if (count == n)
                return sq;
        }
        ++r;
    }
}
