/*
 * A168355  Products of 3 distinct primes whose binary expansion is
 *          palindromic.
 * https://oeis.org/A168355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th odd number whose binary representation
   is a palindrome and which is the product of exactly three distinct odd
   primes.  The routine enumerates binary palindromes in increasing order
   and tests each candidate with factor_u64.  It returns the required
   value for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  For safety return 0 if n == 0. */
    if (n == 0) return 0;

    /* Buffers used for prime factorisation (up to 3 factors per number). */
    uint64_t primes_buf[16];
    uint64_t exps_buf[16];

    size_t found = 0;          /* number of valid terms found so far */

    /* Enumerate binary palindromes by increasing length. */
    for (int len = 1; found < (size_t)n; ++len) {
        int halfBits = (len + 1) / 2;          /* number of bits that define the left half */
        uint64_t minHalf = 1ULL << (halfBits - 1);   /* leading bit must be 1 */
        uint64_t maxHalf = 1ULL << halfBits;          /* exclusive upper bound */

        for (uint64_t h = minHalf; h < maxHalf; ++h) {
            seq_t pal;                       /* candidate palindrome */

            if (len & 1) { /* odd length */
                uint64_t left = h;            /* contains the middle bit as LSB */
                uint64_t rev = 0;
                uint64_t temp = h >> 1;       /* exclude middle bit for mirroring */
                for (int i = 0; i < halfBits - 1; ++i) {
                    rev = (rev << 1) | (temp & 1);
                    temp >>= 1;
                }
                pal = ((seq_t)left << (halfBits - 1)) | (seq_t)rev;
            } else { /* even length */
                uint64_t left = h;
                uint64_t rev = 0;
                uint64_t temp = h;
                for (int i = 0; i < halfBits; ++i) {
                    rev = (rev << 1) | (temp & 1);
                    temp >>= 1;
                }
                pal = ((seq_t)left << halfBits) | (seq_t)rev;
            }

            if (pal <= 1)                /* skip 0 and 1 */
                continue;
            if ((pal & 1) == 0)          /* only odd numbers allowed */
                continue;

            /* Factor the candidate using the 64‑bit factoriser. */
            size_t pcnt = 0;
            factor_u64((uint64_t)pal, primes_buf, exps_buf, &pcnt);
            if (pcnt != 3)                /* exactly three distinct prime factors? */
                continue;

            /* Ensure all factors are distinct odd primes with exponent 1. */
            bool ok = true;
            for (size_t i = 0; i < pcnt; ++i) {
                if (primes_buf[i] == 2 || exps_buf[i] != 1) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                continue;

            /* Found a valid term. */
            ++found;
            if (found == (size_t)n)
                return pal;
        }
    }

    /* The function always returns before reaching this point. */
    return 0;
}
