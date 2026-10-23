/*
 * A135533  Guy Steele's sequence GS(4,6) (see A135416).
 * https://oeis.org/A135533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊log₂(n)⌋} 2^{popcount(n mod 2^k)}
 * Range:     u8 n <= 382, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where ⌊log₂(n)⌋ is the index of the most significant 1-bit of n */
#include <stdint.h>

/* Computes ⌊log₂(n)⌋ for n≥1. */
static inline idx_t floor_log2(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    return (idx_t)(63 - __builtin_clzll(n));
#else
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
#endif
}

/* Computes the binary weight (popcount) of an idx_t value. */
static inline idx_t popcnt_idx(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return (idx_t)__builtin_popcountll(v);
#else
    idx_t c = 0;
    while (v)
    {
        v &= v - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* n ≥ 1 in the sequence definition. */
    idx_t logn = floor_log2(n);
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= logn; ++k)
    {
        /* Compute n mod 2^k by masking. For k=0, mask = 0. */
        idx_t mask = ((idx_t)1 << k) - (idx_t)1;
        idx_t x = n & mask;
        idx_t pc = popcnt_idx(x);

        /* Add 2^pc to the sum. */
        seq_t term = ((seq_t)1 << pc);
        sum += term;
    }
    return sum;
}
