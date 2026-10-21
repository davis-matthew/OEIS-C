/*
 * A066123  Numbers that in base 2 need two 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A066123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numbers that in base 2 need two "Reverse and Add" steps to reach a palindrome.
 * The function enumerates positive integers in increasing order, skipping numbers
 * that are already binary palindromes.  For each candidate, it performs
 *
 *   sum1 = candidate + reverse_bits(candidate)
 *   sum2 = sum1  + reverse_bits(sum1)
 *
 * and keeps the candidate if sum1 is not a palindrome but sum2 is a palindrome.
 * The first such number is 11, as required (a(1)=11).  The routine memoises
 * results so that calls with arbitrary ordering of n remain fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/bitlen_seq_t.h"

/* Reverse the binary representation of a value of type seq_t. */
static seq_t reverse_bits_custom(seq_t v)
{
    unsigned len = bitlen_seq_t(v);
    seq_t r = 0;
    for (unsigned i = 0; i < len; ++i) {
        seq_t bit = (v >> i) & 1;
        r = (r << 1) | bit;
    }
    return r;
}

/* Memoised cache of the sequence values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored */
static idx_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t candidate = 1;   /* next integer to test */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */
    while (cache_len < n) {
        ensure_cache_capacity(cache_len + 1);

        seq_t val = candidate++;
        /* Skip numbers that are already binary palindromes. */
        if (is_palindrome(val, 2)) continue;

        seq_t rev1 = reverse_bits_custom(val);
        seq_t sum1 = val + rev1;
        /* If we reach a palindrome after one step, skip. */
        if (is_palindrome(sum1, 2)) continue;

        seq_t rev2 = reverse_bits_custom(sum1);
        seq_t sum2 = sum1 + rev2;
        if (is_palindrome(sum2, 2)) {
            cache[cache_len++] = val;   /* number needs exactly two steps */
        }
    }
    return cache[n - 1];
}
