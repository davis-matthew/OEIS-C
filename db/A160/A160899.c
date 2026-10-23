/*
 * A160899  a(n) = the smallest positive multiple of n that has exactly a
 *          prime number of 1's in its binary representation.
 * https://oeis.org/A160899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest positive multiple of n that has a prime
 *            number of 1's in its binary representation
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* List of prime numbers up to 128.  Popcounts larger than 127
   cannot occur in a 128‑bit integer. */
static const unsigned long long _prime_popcount[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL,
    37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL,
    79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL
};

/* Check whether a popcount value is prime. */
static bool _is_prime_popcount(unsigned long long c)
{
    for (size_t i = 0;
         i < sizeof(_prime_popcount) / sizeof(_prime_popcount[0]); ++i)
        if (c == _prime_popcount[i]) return true;
    return false;
}

/* Compute the popcount of a seq_t value. */
static unsigned long long _popcount_seq(seq_t x)
{
    unsigned long long cnt = 0;
    while (x != 0)
    {
        if ((x & (seq_t)1) != 0) ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* Sequence function.  For n == 0 a(0) is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Safety guard */
    idx_t k = 1;
    for (;; ++k)
    {
        seq_t m = (seq_t)n * (seq_t)k;
        if (_is_prime_popcount(_popcount_seq(m))) return m;
    }
}
