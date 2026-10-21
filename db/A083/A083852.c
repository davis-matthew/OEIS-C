/*
 * A083852  Decimal palindromes that are multiples of 11.
 * https://oeis.org/A083852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 158, u32 n <= 1908, u64 n <= 1908,
 *            u128 n <= 1908, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083852: decimal palindromes that are multiples of 11.
   a(1)=0, a(2)=11, a(3)=22, a(4)=33, a(5)=44, …   */
#include <stdint.h>
#include <stdbool.h>

/* Reverse the least‑significant `len` decimal digits of `x`.
   Works with `len` equal to zero (returns 0). */
static seq_t rev_digits_uint64(uint64_t x, int len)
{
    seq_t r = 0;
    for (int i = 0; i < len; ++i) {
        r = r * (seq_t)10 + (seq_t)(x % 10);
        x /= 10;
    }
    return r;
}

/* Powers of 10 up to 10^20; 20 is ample for n≤10000. */
static const uint64_t pow10_uint64[21] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
    0ULL
};

seq_t a(idx_t n)
{
    /* Lazily compute powers of 10 as `seq_t` (once). */
    static seq_t pow10[21];
    static int pow10_init = 0;
    if (!pow10_init) {
        pow10[0] = 1;
        for (int i = 1; i < 21; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        pow10_init = 1;
    }

    idx_t count = 0;               /* count of palindromes divisible by 11 encountered */

    for (int len = 1; ; ++len) {   /* exterior digit length, unbounded */
        int half_len = (len + 1) / 2;   /* number of independent digits */
        uint64_t start, end;

        if (len == 1) {
            /* include the single digit 0 for a(1) */
            start = 0;
            end   = 9;
        } else {
            start = pow10_uint64[half_len - 1];   /* avoid leading zeros */
            end   = pow10_uint64[half_len] - 1;
        }

        for (uint64_t left = start; left <= end; ++left) {
            seq_t palindrome;

            if (len % 2 == 0) {                 /* even length   abc…cba */
                seq_t left_seq = (seq_t)left;
                seq_t rev = rev_digits_uint64(left, half_len);
                palindrome = left_seq * pow10[half_len] + rev;
            } else {                            /* odd length    abc…ba */
                seq_t left_seq = (seq_t)left;
                uint64_t left_trim = left / 10;   /* middle digit not mirrored */
                seq_t rev = rev_digits_uint64(left_trim, half_len - 1);
                palindrome = left_seq * pow10[half_len - 1] + rev;
            }

            if (palindrome % (seq_t)11 == 0) {
                ++count;
                if (count == n)
                    return palindrome;
            }
        }
    }
}
