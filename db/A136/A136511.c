/*
 * A136511  Smallest power of n containing it in binary representation.
 * https://oeis.org/A136511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A136511:  a(n) = n^A136510(n)
    a(n) is the least power of n whose binary representation contains
    the binary representation of n as a contiguous substring.
    For n = 1 the answer is 1, otherwise the exponent is at least 2.  */

#include <stdbool.h>
#include <stddef.h>

/* Return the number of bits needed to represent x (x > 0). */
static size_t bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x)
    {
        x >>= 1;
        ++len;
    }
    return len ? len : 1;   /* x is always >0 when used */
}

/* Test whether the binary expansion of `small` appears in the binary
   expansion of `power`.  `small_bits` must be the length of `small`.
   Both values are non‑zero.  */
static bool contains_bits(seq_t power, seq_t small, size_t small_bits)
{
    size_t power_bits = bitlen_seq_t(power);
    if (power_bits < small_bits)
        return false;

    for (size_t pos = 0; pos + small_bits <= power_bits; ++pos)
    {
        bool ok = true;
        for (size_t j = 0; j < small_bits; ++j)
        {
            if (((power >> (pos + j)) & 1) != ((small >> j) & 1))
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* offset 1 */
    if (n == 1)        /* a(1) = 1^1 = 1 */
        return (seq_t)1;

    seq_t small = (seq_t)n;                /* binary representation of n */
    size_t small_bits = bitlen_seq_t(small);

    /* start with the first non‑trivial power, n^2 */
    seq_t power = small * small;           /* n^2 */
    while (true)
    {
        if (contains_bits(power, small, small_bits))
            return power;

        /* next power: power *= n */
        power = power * small;
    }
}
