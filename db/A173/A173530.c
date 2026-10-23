/*
 * A173530  Number of ON cells after n generations of three-dimensional
 *          cellular automaton related to Sierpinski's triangle and the
 *          toothpick sequences (See Comments for definition).
 * https://oeis.org/A173530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} 2^(popcnt(k)+popcnt(k-1)-1) where popcnt(x) is
 *            the number of set bits in the binary representation of x. This
 *            is based on the OEIS definition of the 3‑D cellular automaton
 *            described in A173530
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of 1‑bits in a 64‑bit integer. */
static inline uint64_t popcnt_u64(uint64_t x)
{
    uint64_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        uint64_t p   = popcnt_u64(k);
        uint64_t pp  = popcnt_u64(k - 1);
        uint64_t exp = p + pp - 1;          /* exponent is always ≥ 0 */
        seq_t term   = (seq_t)((uint64_t)1 << exp);
        sum += term;
    }

    return sum;
}
