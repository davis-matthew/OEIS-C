/*
 * A084991  Palindromes which can be written as a sum of two prime numbers.
 * https://oeis.org/A084991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084991:  a(n) is the nth palindrome that can be written as a sum of two primes.
   Odd palindromes are kept iff the number minus two is prime; all even palindromes >2 are kept.
   Implementation generates palindromes in order and tests them. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

static idx_t reverse_digits(idx_t v)
{
    idx_t rev = 0;
    while (v > 0) {
        rev = rev * 10 + (v % 10);
        v /= 10;
    }
    return rev;
}

static seq_t make_pal(idx_t len, idx_t half)
{
    unsigned long long result = 0;
    idx_t half_len = (len + 1) / 2;

    if (len % 2 == 0) {                    /* even length */
        unsigned long long left = half;
        unsigned long long rev  = reverse_digits(half);
        unsigned long long pow  = 1;
        for (idx_t i = 0; i < half_len; ++i) pow *= 10ULL;
        result = left * pow + rev;
    } else {                                /* odd length */
        unsigned long long left_val = half / 10;
        unsigned long long middle   = half % 10;
        unsigned long long rev      = reverse_digits(left_val);
        unsigned long long pow_half = 1;
        for (idx_t i = 0; i < half_len; ++i) pow_half *= 10ULL;
        unsigned long long pow_mid = 1;
        for (idx_t i = 0; i < half_len - 1; ++i) pow_mid *= 10ULL;
        result = left_val * pow_half + middle * pow_mid + rev;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;          /* number of valid palindromes stored */

    /* state for palindrome generation */
    static idx_t cur_len = 1;
    static idx_t cur_half_min = 1;
    static idx_t cur_half_max = 9;
    static idx_t cur_half = 1;

    if (cache_len < n) {
        /* ensure enough capacity */
        size_t required = n;
        if (cache_cap < required) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure: sentinel */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* generate palindromes until we have at least n terms */
        while (cache_len < n) {
            seq_t pal = make_pal(cur_len, cur_half);

            if (pal >= (seq_t)4) {          /* ignore smaller numbers */
                if ((pal % (seq_t)2) == 0) {    /* even palindrome >2 */
                    cache[cache_len++] = pal;
                } else {                       /* odd palindrome */
                    if (isPrimeGeneric(pal - (seq_t)2))
                        cache[cache_len++] = pal;
                }
            }

            /* advance to next palindrome */
            if (cur_half == cur_half_max) {
                ++cur_len;
                idx_t half_len = (cur_len + 1) / 2;
                cur_half_min = (cur_len == 1) ? 1 : pow10_idx(half_len - 1);
                cur_half_max = pow10_idx(half_len) - 1;
                cur_half = cur_half_min;
            } else {
                ++cur_half;
            }
        }
    }

    return cache[n - 1];
}
