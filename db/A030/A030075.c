/*
 * A030075  Squares which are palindromes in base 15.
 * https://oeis.org/A030075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth square that is a palindrome in base 15
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 65, u64 n <= 106, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* The next integer whose square is to be tested. */
static idx_t next_root = 0;

#ifdef __cplusplus
/* In C++ a std::vector handles dynamic growth automatically. */
static std::vector<seq_t> cache;
#else
/* In C we allocate a raw array of seq_t, resizing as needed. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
#endif

#ifndef __cplusplus
/* Grow the cache array so it has at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache       = new_ptr;
    cache_cap   = new_cap;
}
#endif

/* Store a new term into the cache. */
static void add_to_cache(seq_t val)
{
#ifdef __cplusplus
    cache.push_back(val);
#else
    ensure_cache_cap(cache_size + 1);
    cache[cache_size++] = val;
#endif
}

/* Return true if 'x' is a palindrome when written in base 15. */
static bool is_palindrome_base15(seq_t x)
{
    if (x == 0) return true;
    uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 15);
        tmp /= 15;
    }
    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i]) return false;
    }
    return true;
}

/* Return the nth term of A030075.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1, return 0. */
    while (1) {
#ifdef __cplusplus
        size_t have = cache.size();
#else
        size_t have = cache_size;
#endif
        if (have >= (size_t)n) break;
        seq_t root = (seq_t)next_root;
        seq_t sq   = root * root;
        if (is_palindrome_base15(sq)) add_to_cache(sq);
        ++next_root;
    }
#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[n - 1];
#endif
}
