/*
 * A045638  Palindromic and divisible by 3.
 * https://oeis.org/A045638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 251, u32 n <= 2001, u64 n <= 2001,
 *            u128 n <= 2001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth decimal palindrome divisible by 3. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t *cache = NULL;
static uint64_t next_len = 1;          /* length of next palindromes to generate */

/* pre‑computed powers of ten 10^i  (i < 64) */
static seq_t pow10_arr[64];
static size_t pow10_cap = 1;           /* we have pow10_arr[0] ready */

static void ensure_pow10(size_t max_exp)
{
    /* populate pow10_arr up to max_exp */
    if (max_exp < pow10_cap) return;
    for (size_t i = pow10_cap; i <= max_exp; ++i) {
        pow10_arr[i] = pow10_arr[i-1] * (seq_t)10;
    }
    pow10_cap = max_exp + 1;
}

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* reverse the lowest k decimal digits of x */
static seq_t reverse_digits(seq_t x, unsigned k)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < k; ++i) {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x /= (seq_t)10;
    }
    return rev;
}

/* generate all palindromes of a given decimal length
   that are divisible by 3, and append them to the cache
   in ascending order. */
static void generate_length(unsigned len)
{
    ensure_pow10(len + 2);          /* we may need 10^(len+1) for odd lengths */

    if (len == 1) {
        for (seq_t d = 0; d <= 9; ++d) {
            seq_t val = d;
            if (val % (seq_t)3 == 0) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
        return;
    }

    if ((len & 1U) == 0) {          /* even length  = 2k */
        unsigned k = len >> 1;
        seq_t start = pow10_arr[k-1];
        seq_t end   = pow10_arr[k] - (seq_t)1;
        for (seq_t base = start; base <= end; ++base) {
            seq_t rev = reverse_digits(base, k);
            seq_t val = base * pow10_arr[k] + rev;
            if (val % (seq_t)3 == 0) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
    } else {                         /* odd length  = 2k+1  (k ≥ 1) */
        unsigned k = (len - 1) >> 1;
        seq_t start = pow10_arr[k-1];
        seq_t end   = pow10_arr[k] - (seq_t)1;
        for (seq_t base = start; base <= end; ++base) {
            seq_t rev = reverse_digits(base, k);
            for (seq_t mid = 0; mid <= 9; ++mid) {
                seq_t val = base * pow10_arr[k+1] + mid * pow10_arr[k] + rev;
                if (val % (seq_t)3 == 0) {
                    ensure_cache_capacity(cache_len + 1);
                    cache[cache_len++] = val;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety guard */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        pow10_arr[0] = (seq_t)1;                /* initialise powers of ten */
    }

    /* Extend cache until it contains the required term */
    while (cache_len < (size_t)n) {
        generate_length((unsigned)next_len);
        ++next_len;
    }

    return cache[(size_t)n - 1];
}
