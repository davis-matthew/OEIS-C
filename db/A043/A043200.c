/*
 * A043200  Numbers k such that 2 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A043200:
   a(n) = the n-th integer k such that the base-9 representation of k contains
   the adjacent digit pair 28 or 82, but the base-9 representation of k-1 does NOT.
   The sequence starts 26, 74, 107, 155, … .  This implementation builds
   a cache lazily and works for any unsigned integer type seq_t (8, 16, 32, 64,
   128 bits) as well as an arbitrary‑precision integer class (C++17). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return true if x in base 9 contains 28 or 82 as consecutive digits. */
static bool has_adjacent_2_8(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 9);          /* digit in the current place */
        if ((d == 2 && prev == 8) || (d == 8 && prev == 2))
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Compute the n-th term of the sequence, n >= 1.  For n == 0 return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for index 0 */

#ifdef __cplusplus
    /* C++ implementation using std::vector for the cache. */
    static std::vector<seq_t> cache;
    static seq_t next_k = 1;          /* next integer to check */

    while (cache.size() < (size_t)n) {
        if (has_adjacent_2_8(next_k) && !has_adjacent_2_8(next_k - 1))
            cache.push_back(next_k);
        ++next_k;
    }
    return cache[n - 1];
#else
    /* C implementation using a dynamic array. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    static seq_t next_k = 1;

    while (cache_size < (size_t)n) {
        if (has_adjacent_2_8(next_k) && !has_adjacent_2_8(next_k - 1)) {
            if (cache_size == 0) {
                cache_capacity = 8;
                cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
            } else if (cache_size >= cache_capacity) {
                cache_capacity *= 2;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
#endif
}
