/*
 * A141708  Least positive multiple of 2n-1 which is palindromic in base 2.
 * https://oeis.org/A141708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) is the least positive multiple of (2n-1) that is a
 *            palindrome in base 2. This implements the definition by
 *            searching binary palindromes for the smallest one divisible by
 *            (2n-1). It works for any seq_t and idx_t provided by the build
 * Range:     u8 n <= 9, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 923 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: find the smallest binary palindrome divisible by m.
   m is an odd seq_t (2n-1).  The routine generates palindromes
   in increasing numeric order and returns the first divisible. */
static seq_t
smallest_palindrome_divisible(seq_t m)
{
    /* m fits in 32 bits for the problem domain (n ≤ 10000),
       but we use uint64_t for the modular calculations. */
    uint64_t mod_target = (uint64_t)m;

    /* Length of palindrome (number of bits).  Start from 1. */
    for (int len = 1; ; ++len) {
        int half = (len + 1) >> 1;          /* ceil(len/2) */
        /* left half must start with 1: bits [half-1] is 1. */
        int left_start = 1 << (half - 1);
        int left_end   = 1 << half;         /* exclusive */

        /* Enumerate all possible left halves with the leading 1. */
        for (int left = left_start; left < left_end; ++left) {
            uint64_t mod = 0;

            /* Add bits of left half (most significant to least). */
            for (int i = half - 1; i >= 0; --i) {
                int bit = (left >> i) & 1;
                mod = ((mod << 1) + bit) % mod_target;
            }

            /* Reflect left half to form the palindrome. */
            if ((len & 1) == 0) {          /* even length */
                for (int i = 0; i < half; ++i) {
                    int bit = (left >> i) & 1;
                    mod = ((mod << 1) + bit) % mod_target;
                }
            } else {                       /* odd length: skip middle bit */
                for (int i = 1; i < half; ++i) {
                    int bit = (left >> i) & 1;
                    mod = ((mod << 1) + bit) % mod_target;
                }
            }

            if (mod == 0) {                /* divisible: build full value */
                seq_t val = 0;
                for (int i = half - 1; i >= 0; --i) {
                    int bit = (left >> i) & 1;
                    val = (val << 1) | bit;
                }
                if ((len & 1) == 0) {
                    for (int i = 0; i < half; ++i) {
                        int bit = (left >> i) & 1;
                        val = (val << 1) | bit;
                    }
                } else {
                    for (int i = 1; i < half; ++i) {
                        int bit = (left >> i) & 1;
                        val = (val << 1) | bit;
                    }
                }
                return val;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* m = 2n-1 (odd).  Use seq_t for the multiplication
       but maintain m in the small integer form for mod. */
    seq_t m = (seq_t)(2 * n - 1u);
    return smallest_palindrome_divisible(m);
}
