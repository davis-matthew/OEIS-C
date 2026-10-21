/*
 * A084353  Palindromes such that the product of the nonzero digits + 1 is
 *          prime.
 * https://oeis.org/A084353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 280, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A084353
   a(n) is the n-th palindrome (in increasing order) such that
   the product of its non‑zero digits plus 1 is a prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static const uint64_t pow10[20] = {
    1ULL,10ULL,100ULL,1000ULL,10000ULL,100000ULL,1000000ULL,
    10000000ULL,100000000ULL,1000000000ULL,10000000000ULL,
    100000000000ULL,1000000000000ULL,10000000000000ULL,
    100000000000000ULL,1000000000000000ULL,10000000000000000ULL,
    100000000000000000ULL,1000000000000000000ULL,10000000000000000000ULL
};

/* build a palindrome from prefix and parity flag.
   If odd==false the whole prefix is mirrored.
   If odd==true the lowest digit of prefix is skipped when mirroring. */
static inline uint64_t make_pal(uint64_t prefix, bool odd)
{
    uint64_t pal = prefix;
    uint64_t rev = odd ? prefix / 10 : prefix;
    while (rev > 0) {
        pal = pal * 10 + rev % 10;
        rev /= 10;
    }
    return pal;
}

/* product of non‑zero digits of x */
static inline uint64_t digit_prod(uint64_t x)
{
    uint64_t prod = 1;
    while (x > 0) {
        uint64_t d = x % 10;
        if (d != 0) prod *= d;
        x /= 10;
    }
    return prod;
}

/* true if palindrome p satisfies the condition */
static inline bool good_pal(uint64_t p)
{
    uint64_t prod = digit_prod(p);
    uint64_t test = prod + 1;
    return is_prime_u64(test);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    /* handle lengths from 1 upward */
    for (int len = 1; ; ++len) {
        if (len == 1) {
            for (uint64_t d = 1; d <= 9; ++d) {
                if (good_pal(d)) {
                    ++count;
                    if (count == n) return (seq_t)d;
                }
            }
        } else {
            int half = len / 2;
            uint64_t start = pow10[half - 1];
            uint64_t end   = pow10[half];
            if (len % 2 == 0) { /* even length */
                for (uint64_t pref = start; pref < end; ++pref) {
                    uint64_t p = make_pal(pref, false);
                    if (good_pal(p)) {
                        ++count;
                        if (count == n) return (seq_t)p;
                    }
                }
            } else { /* odd length */
                for (uint64_t pref = start; pref < end; ++pref) {
                    for (uint64_t mid = 0; mid <= 9; ++mid) {
                        uint64_t prefix = pref * 10 + mid;
                        uint64_t p = make_pal(prefix, true);
                        if (good_pal(p)) {
                            ++count;
                            if (count == n) return (seq_t)p;
                        }
                    }
                }
            }
        }
    }
}
