/*
 * A030675  Smallest nontrivial extension of n-th palindrome which is a prime.
 * https://oeis.org/A030675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A030665(A002113(n+1)) Definition: a(n) is the smallest
 *            prime that extends the (n+1)-st decimal palindrome (the prefix),
 *            using the shortest extension that yields a prime. The expansion
 *            is allowed to add digits at the end only
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 66, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"

/* square‑free conversion to seq_t is not needed – the helper expects seq_t */
static seq_t is_prime_check(seq_t x) { return is_prime_big(x); }

/* compute 10ⁿ (uint64_t) for n <= 18 */
static uint64_t pow10_uint(uint64_t e)
{
    uint64_t r = 1;
    while (e--) r *= 10ULL;
    return r;
}

/* reverse decimal digits of x (uint64_t) */
static uint64_t rev_digits(uint64_t x)
{
    uint64_t r = 0;
    while (x > 0) {
        r = r * 10ULL + (x % 10ULL);
        x /= 10ULL;
    }
    return r;
}

/* build the palindrome of total length 'len' from prefix 'pref'.
   'len' >= 1, 'pref' has exactly ceil(len/2) digits. */
static uint64_t build_pal(uint64_t pref, uint64_t len)
{
    uint64_t half = (len + 1) / 2;          /* digits in pref */
    if (len & 1ULL) {                       /* odd length */
        uint64_t power = pow10_uint(half - 1ULL);
        uint64_t rev = rev_digits(pref / 10ULL);
        return pref * power + rev;
    } else {                                /* even length */
        uint64_t power = pow10_uint(half);
        uint64_t rev = rev_digits(pref);
        return pref * power + rev;
    }
}

/* return the (n+1)-th decimal palindrome (1‑based).  n >= 1. */
static uint64_t nth_palindrome(idx_t n)
{
    uint64_t remaining = (uint64_t)n;
    for (uint64_t len = 1ULL; ; ++len) {
        uint64_t count;
        if (len == 1ULL) {
            count = 9ULL;                 /* 1‑9 */
        } else {
            uint64_t half = (len + 1ULL) / 2ULL;
            count = 9ULL * pow10_uint(half - 1ULL);
        }
        if (remaining <= count) {
            uint64_t half = (len + 1ULL) / 2ULL;
            uint64_t start = pow10_uint(half - 1ULL);
            uint64_t offset = remaining - 1ULL;   /* 0‑based */
            uint64_t pref = start + offset;
            return build_pal(pref, len);
        }
        remaining -= count;
    }
}

/* find the smallest prime whose decimal representation has 'p' as a prefix
   (p >= 1). */
static uint64_t prime_extension(uint64_t p)
{
    uint64_t pow10 = 10ULL;
    while (1) {
        uint64_t base  = p * pow10;
        uint64_t limit = base + pow10 - 1ULL;
        for (uint64_t cand = base; cand <= limit; ++cand) {
            if (is_prime_check((seq_t)cand))
                return cand;
        }
        pow10 *= 10ULL;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based, apply formula: a(n) = A030665(A002113(n+1)) */
    uint64_t p = nth_palindrome(n);
    return (seq_t)prime_extension(p);
}
