/*
 * A056749  Smallest palindrome greater than n in bases 2 and n.
 * https://oeis.org/A056749
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 132, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome, pal_from_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056749: smallest integer m > n that is a palindrome in base 2
   and in base n.  For each n ≥ 2, search over binary palindromes in
   increasing order and test the base‑n palindrome property. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"
#include "../utils/pal_from_prefix.h"

/* reverse bits of a number in the given bit width */
static inline uint64_t rev_bits(uint64_t x, unsigned bits)
{
    uint64_t r = 0;
    while (bits--)
    {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* start with the shortest possible binary palindrome (length 2) */
    for (unsigned L = 2; ; ++L)
    {
        unsigned k = (L + 1) / 2;                /* half-length (rounded up) */
        if (k == 0) continue;                   /* just in case */
        uint64_t start = 1ULL << (k - 1);        /* leading bit must be 1 */
        uint64_t end   = (1ULL << k) - 1;
        for (uint64_t pref = start; pref <= end; ++pref)
        {
            seq_t m = pal_from_prefix(pref, k, L);
            if (m <= (seq_t)n) continue;        /* we want m > n */
            if (is_palindrome(m, n))            /* base‑n palindrome test */
                return m;
        }
    }
}
