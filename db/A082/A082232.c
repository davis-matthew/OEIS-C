/*
 * A082232  Palindromes divisible by their digit sum.
 * https://oeis.org/A082232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082232: a(n) is the n-th palindrome divisible by its digit sum.
   Computation is performed by enumerating palindromes in increasing order
   and testing divisibility by the digit sum. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Pre‑computed powers of 10 up to 10^20.
   Returns 10^exp as uint64_t. */
static uint64_t pow10_uint64(int exp)
{
    static uint64_t p10[21] = {0};
    if (p10[0] == 0) {
        p10[0] = 1ULL;
        for (int i = 1; i < 21; ++i) {
            p10[i] = p10[i - 1] * 10ULL;
        }
    }
    return p10[exp];
}

/* Reverse the decimal digits of a uint64_t. */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x != 0) {
        rev = rev * 10ULL + (x % 10ULL);
        x /= 10ULL;
    }
    return rev;
}

/* Static cache of discovered terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t total_found = 0;

/* State used to generate palindromes in order. */
static idx_t cur_len = 1;          /* current palindrome length */
static uint64_t cur_prefix_start = 1ULL;
static uint64_t cur_prefix_end   = 9ULL;
static uint64_t cur_prefix       = 1ULL;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize the generator for a given palindrome length. */
static void set_length(idx_t len)
{
    cur_len = len;
    int half = (int)(len >> 1);            /* floor(len/2) */
    if (len == 1) {
        cur_prefix_start = 1ULL;
        cur_prefix_end   = 9ULL;
    } else if ((len & 1) == 0) {           /* even length */
        cur_prefix_start = pow10_uint64(half - 1);
        cur_prefix_end   = pow10_uint64(half) - 1ULL;
    } else {                               /* odd length */
        cur_prefix_start = pow10_uint64(half);
        cur_prefix_end   = pow10_uint64(half + 1) - 1ULL;
    }
    cur_prefix = cur_prefix_start;
}

/* Generate palindromes until we have at least 'target' terms. */
static void generate_up_to(idx_t target)
{
    while (total_found < target) {
        if (cur_prefix > cur_prefix_end) {
            ++cur_len;
            set_length(cur_len);
            continue;
        }

        uint64_t pal;
        if (cur_len == 1) {
            pal = cur_prefix;
        } else if ((cur_len & 1) == 0) {      /* even length */
            int half = (int)(cur_len >> 1);
            pal = cur_prefix * pow10_uint64(half) + reverse_uint64(cur_prefix);
        } else {                               /* odd length */
            int half = (int)(cur_len >> 1);
            uint64_t tmp = cur_prefix / 10ULL;    /* drop middle digit */
            pal = cur_prefix * pow10_uint64(half) + reverse_uint64(tmp);
        }

        seq_t pal_seq = (seq_t)pal;
        seq_t ds = digit_sum(pal_seq);
        if (ds != 0 && pal_seq % ds == 0) {
            ensure_cache_cap(total_found + 1);
            cache[total_found] = pal_seq;
            ++total_found;
        }
        ++cur_prefix;
    }
}

/* Must be called with n >= 1; index is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    generate_up_to(n);
    return cache[n - 1];
}
