/*
 * A185351  Sums of distinct perfect numbers.
 * https://oeis.org/A185351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ (p_i * bit_i) where bit_i are bits of (n-1). p_0 = 6,
 *            p_1 = 28, p_2 = 496, … are the distinct perfect numbers
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 32, u64 n <= 256, u128 n <= 512,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Known Mersenne prime exponents (enough for the first 64 perfect numbers). */
static const idx_t _mersenne_exp[64] = {
    2, 3, 5, 7, 13, 17, 19, 31,
    61, 89, 107, 127, 521, 607,
    1279, 2203, 2281, 3217, 4253, 4423,
    9689, 9941, 11213, 19937, 21701,
    23209, 44497, 86243, 110503, 132049,
    216091, 756839, 859433, 1257787, 1398269,
    2976221, 3021377, 6972593, 13466917,
    20996011, 24036583, 25964951, 30402457,
    32582657, 37156667
};

/* Cache of perfect numbers that have been computed so far. */
static seq_t _perfect[64];
static size_t _perfect_count = 0;

/* Compute 2^exp as seq_t, safe for small and large exponents. */
static seq_t _pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 2;
    return r;
}

/* Compute the perfect number corresponding to the Mersenne exponent p. */
static seq_t _perfect_from_exponent(idx_t p)
{
    seq_t power_p_minus_1 = _pow2(p - 1);
    seq_t power_p      = _pow2(p);
    return power_p_minus_1 * (power_p - 1);
}

/* Ensure that perfect[k] exists for all k < idx.  Generates further
   perfect numbers on demand.  The array size prevents overrun for
   n < 2^64. */
static void _ensure_perfects(size_t idx)
{
    if (idx <= _perfect_count)
        return;
    if (idx > sizeof(_mersenne_exp)/sizeof(_mersenne_exp[0]))
        return; /* cannot compute more exponents */
    for (; _perfect_count < idx; ++_perfect_count)
        _perfect[_perfect_count] = _perfect_from_exponent(_mersenne_exp[_perfect_count]);
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0. */
    if (n <= 1)
        return (seq_t)0;

    idx_t bits = n - 1;
    seq_t sum = 0;
    size_t bitpos = 0;

    while (bits)
    {
        if (bits & 1)
        {
            _ensure_perfects(bitpos + 1);
            if (bitpos < _perfect_count)
                sum += _perfect[bitpos];
        }
        bits >>= 1;
        ++bitpos;
    }
    return sum;
}
