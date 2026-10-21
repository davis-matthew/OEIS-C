/*
 * A072601  Numbers which in base 2 have at least as many 1's as 0's.
 * https://oeis.org/A072601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 5370, u32 n <= 5370, u64 n <= 5370,
 *            u128 n <= 5370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A072601 – Numbers which in base 2 have at least as many 1's as 0's.
    Computation: iterate integers from 1 upwards, keeping those whose
    binary representation satisfies popcount(x) ≥ bitlen(x)/2, until the
    requested n‑th term is found.  No caching is required for n up to
    at least 10 000. */
#include <stdbool.h>
#include <stddef.h>

/* popcount for a 64‑bit value (used for checking the condition; the
   search values stay below 2⁶⁴ for the required range). */
static inline unsigned popcount64(uint64_t v)
{
    return __builtin_popcountll(v);
}

/* number of bits needed to represent v (>0). */
static inline unsigned bitlen64(uint64_t v)
{
    return v ? 64 - __builtin_clzll(v) : 1;
}

/* true if v has at least as many 1 bits as 0 bits. */
static inline bool good64(uint64_t v)
{
    unsigned ones = popcount64(v);
    unsigned len  = bitlen64(v);
    return (ones << 1) >= len;               /* avoid division */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but harmless */

    idx_t found = 0;          /* how many good numbers seen */
    uint64_t val = 0;         /* current tested integer */

    /* search until we find the n‑th good number */
    while (found < n)
    {
        ++val;                       /* candidate integer */
        if (good64(val))
            ++found;
    }
    /* cast to seq_t – the value fits into idx_t and thus into seq_t */
    return (seq_t)val;
}
