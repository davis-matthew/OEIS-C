/*
 * A136916  Numbers k such that k and k^2 use only the digits 0, 2, 5, 7 and
 *          8.
 * https://oeis.org/A136916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 431, u64 n <= 1045,
 *            u128 n <= 1045, big any n
 * Validated: standard, pass, 68 terms through n = 805 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find k such that k and k^2 use only digits 0, 2, 5, 7, and 8.
   The sequence is generated in increasing numerical order by
   recursively constructing all numbers that consist solely of the
   allowed digits and testing whether the square also satisfies the
   digit condition.  Results are memoised in a static array so that
   any call to a(n) is O(1) after the required terms have been
   generated.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Allowed digit test for __int128 numbers (used for squares). */
static bool allowed_uint128(unsigned __int128 x)
{
    if (x == 0) return true;
    while (x) {
        int d = (int)(x % 10);
        if (d != 0 && d != 2 && d != 5 && d != 7 && d != 8) return false;
        x /= 10;
    }
    return true;
}

/* Dynamic array to hold computed sequence values. */
static seq_t *seq_cache   = NULL;   /* pointer to array of seq_t values */
static size_t  seq_len    = 0;      /* number of values stored */
static size_t  seq_cap    = 0;      /* current capacity of the array */

/* Ensure the ability to store at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    if (new_cap < need) new_cap = need;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap   = new_cap;
}

/* Append a new value to the sequence cache. */
static void append_seq(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Recursive generator for numbers of a given length composed of
   the allowed digits in ascending numeric order. */
static void generate_len_rec(unsigned int pos, uint64_t num, unsigned int len)
{
    if (pos == len) {
        /* Test whether the square also uses only allowed digits. */
        unsigned __int128 sq = (unsigned __int128)num * (unsigned __int128)num;
        if (allowed_uint128(sq))
            append_seq((seq_t)num);
        return;
    }
    /* First position: digits 2,5,7,8 (no leading 0). */
    static const int first_digits[4] = {2,5,7,8};
    /* Subsequent positions: digits 0,2,5,7,8. */
    static const int all_digits[5] = {0,2,5,7,8};
    const int *digits = (pos == 0) ? first_digits : all_digits;
    int count = (pos == 0) ? 4 : 5;
    for (int i = 0; i < count; ++i) {
        generate_len_rec(pos + 1, num * 10 + digits[i], len);
    }
}

/* Generate sequence values until at least 'n' terms are stored.
   The function preserves state between calls. */
static void generate_up_to(size_t n)
{
    /* Ensure the presence of 0 as the very first term (a(1)). */
    if (seq_len == 0 && n > 0) {
        append_seq((seq_t)0);
    }

    /* Generate numbers of increasing length until we have enough terms. */
    static unsigned int max_len_generated = 0;
    while (seq_len < n) {
        ++max_len_generated;
        generate_len_rec(0, 0, max_len_generated);
    }
}

/* Main sequence routine.  Returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    generate_up_to((size_t)n);
    return seq_cache[n - 1];
}
