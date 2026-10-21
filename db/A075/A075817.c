/*
 * A075817  Palindromic even numbers with an odd number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A075817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th palindrome in base 10 that is even and has an odd number of prime factors (counted with multiplicity) */

#include <stdint.h>
#include "../utils/bigomega.h"

/* Reverse an unsigned integer (assumes 64‑bit) */
static idx_t rev_uint64(idx_t n)
{
    idx_t r = 0;
    while (n) {
        r = r * 10 + (n % 10);
        n /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* single‑digit even palindromes (2,4,6,8) */
    static const idx_t firstDigits[4] = {2, 4, 6, 8};
    idx_t count = 0;
    for (int i = 0; i < 4; ++i) {
        idx_t d = firstDigits[i];
        /* d fits in seq_t */
        seq_t val = (seq_t)d;
        if ((bigomega((idx_t)d) & 1) != 0) {
            ++count;
            if (count == n)
                return val;
        }
    }

    /* palindromes of length >= 2 */
    for (int len = 2; ; ++len) {
        int halfLen = len / 2;
        /* compute 10^(halfLen-1) to use as step multiplier for the remaining digits */
        idx_t powHalfPlus = 1;
        for (int p = 1; p < halfLen; ++p)
            powHalfPlus *= 10;

        idx_t powHalf = powHalfPlus * 10;          /* 10^halfLen   */
        idx_t powHalf10 = powHalf * 10;           /* 10^(halfLen+1) for odd lengths */

        for (int k = 0; k < 4; ++k) {                  /* first digit must be even */
            idx_t firstDigit = firstDigits[k];
            idx_t firstHalfStart = firstDigit * powHalfPlus;

            for (idx_t rest = 0; rest < powHalfPlus; ++rest) {
                idx_t firstHalf = firstHalfStart + rest;
                idx_t rev = rev_uint64(firstHalf);

                if ((len & 1) == 0) {    /* even length */
                    seq_t pal = (seq_t)firstHalf * powHalf + rev;
                    if ((bigomega((idx_t)pal) & 1) != 0) {
                        ++count;
                        if (count == n)
                            return pal;
                    }
                } else {                 /* odd length */
                    for (idx_t m = 0; m < 10; ++m) {
                        seq_t pal = (seq_t)firstHalf * powHalf10
                                  + (seq_t)m * powHalf
                                  + rev;
                        if ((bigomega((idx_t)pal) & 1) != 0) {
                            ++count;
                            if (count == n)
                                return pal;
                        }
                    }
                }
            }
        }
    }
}
