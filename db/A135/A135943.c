/*
 * A135943  Numbers with a digit-sum which is a multiple of 3 and with sorted
 *          digits forming an arithmetic progression.
 * https://oeis.org/A135943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135943  
 * Numbers whose digit sum is a multiple of 3 and whose sorted digits
 * (discarding any leading zeros) form an arithmetic progression. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

/* Test whether a number satisfies the defining property.   */
static bool is_valid(seq_t x)
{
    if (x == 0) return false;                    /* 0 is not in the sequence */

    /* Count digits and compute digit sum modulo 3.  */
    seq_t t = x;
    int sum_mod3 = 0;
    unsigned char count[10] = {0};

    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);
        count[d]++;
        sum_mod3 += d;
        t = t / 10;
    }
    if (sum_mod3 % 3 != 0) return false;

    /* Build a sorted list of the digits.  The maximum number of
       decimal digits in the operands that will ever be handled
       comfortably by the algorithm is far below 200.           */
    unsigned char digits[200];
    size_t len = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned char c = 0; c < count[d]; ++c) {
            digits[len++] = (unsigned char)d;
        }
    }

    /* Remove leading zeros in the sorted list.            */
    size_t start = 0;
    while (start < len && digits[start] == 0) ++start;
    size_t new_len = len - start;
    if (new_len <= 1) return true;                 /* one digit → always AP */

    unsigned char diff = digits[start + 1] - digits[start];
    for (size_t i = start + 2; i < len; ++i) {
        if (digits[i] - digits[i - 1] != diff) return false;
    }
    return true;
}

/* The main sequence function.  This function is memoised for efficiency. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for n = 0 */

    /* Static cache of already discovered terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;          /* the next number to test */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        if (is_valid(next_candidate)) {
            /* Make room for a new term if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 16;
#ifdef __cplusplus
                seq_t *new_cache = new seq_t[new_cap];
                for (idx_t i = 0; i < cache_len; ++i) {
                    new_cache[i] = cache[i];
                }
                delete[] cache;
                cache = new_cache;
                cache_cap = (idx_t)new_cap;
#else
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = (idx_t)new_cap;
#endif
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
