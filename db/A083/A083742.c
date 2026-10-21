/*
 * A083742  First differences of A006282.
 * https://oeis.org/A083742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(2n) = a(n) + floor(log₂(2n)); a(2n+1) = a(n) + 1.
 *            (Here n ≥ 0 and floor(log₂(·)) is the integer part of the base‑2
 *            logarithm.)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the floor of log₂ of a positive uint64_t.
   The function is safe for all values of idx_t (uint64_t). */
static inline unsigned floor_log2_uint64(uint64_t x)
{
    unsigned r = 0;
    while (x > 1)
    {
        x >>= 1;
        ++r;
    }
    return r;
}

/* Main sequence function.  The types seq_t and idx_t are defined
   by the build system (unsigned integral types, possibly big). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((n & 1ULL) == 0)          /* even: n = 2·m */
    {
        seq_t m_val = a(n >> 1);          /* a(m) */
        unsigned lg = floor_log2_uint64(n);/* floor(log₂(n)) == floor(log₂(2·m)) */
        return m_val + (seq_t)lg;
    }
    else                            /* odd: n = 2·m + 1 */
    {
        seq_t m_val = a(n >> 1);          /* a(m) */
        return m_val + 1;
    }
}
