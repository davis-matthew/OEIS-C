/*
 * A029738  Palindromic squares in base 12.
 * https://oeis.org/A029738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 44, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 57 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: palindromic squares in base 12 (A029738)
   Computes the nth term by iterating over integer roots,
   squaring them, and keeping those squares whose base‑12
   representation is a palindrome.  The results are cached
   so that subsequent calls with arbitrary n are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* stored a(1)..a(k) */
static size_t cache_cap = 0;         /* capacity of cache */
static size_t cache_len = 0;         /* number of terms cached */
static idx_t   next_root = 0;        /* next integer root to test */

static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    /* cast needed for C++ compilation */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe fallback. */
    if (n < 1) return (seq_t)0;

    size_t target = (size_t)n;
    while (cache_len < target) {
        seq_t root = (seq_t)next_root;
        seq_t sq   = root * root;
        if (is_palindrome_base(sq, 12)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_root;
    }
    return cache[n - 1];
}
