/*
 * A045037  Numbers whose base-4 representation contains exactly four 0's and
 *          four 1's.
 * https://oeis.org/A045037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 4800, u64 n <= 4800,
 *            u128 n <= 4800, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A045037: numbers whose base‑4 representation contains
   exactly four 0's and four 1's.  The sequence is generated in ascending
   order by enumerating all admissible base‑4 strings length by length. */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t next_length_to_generate = 8;

/* Append a new term to the cache, expanding storage if necessary. */
static inline void push_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap << 1 : 16);
        seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/*
 * Recursive generator:
 *   pos          : current digit position (0‑based from left, leading digit already
 *                  processed when pos > 0)
 *   val          : current accumulated value
 *   zeros_left   : zeros still to place
 *   ones_left    : ones still to place
 *   L            : total length of the base‑4 string
 *
 * Digits are tried in ascending order, yielding numbers in numeric order.
 */
static void gen_rec(idx_t pos, seq_t val, int zeros_left, int ones_left, idx_t L)
{
    if (pos == L) {
        if (zeros_left == 0 && ones_left == 0)
            push_value(val);
        return;
    }

    for (int d = 0; d <= 3; ++d) {
        if (d == 0 && zeros_left == 0) continue;
        if (d == 1 && ones_left == 0) continue;
        int nz = zeros_left - (d == 0);
        int no = ones_left  - (d == 1);
        gen_rec(pos + 1, val * 4 + d, nz, no, L);
    }
}

/* Generate all admissible numbers of a given base‑4 length L. */
static void gen_length(idx_t L)
{
    if (!cache)
        cache = seq_alloc(16);

    if (L == 8) {
        /* Leading digit fixed to 1; remaining 7 digits must contain 4 zeros,
           3 ones and no 2/3 digits. */
        gen_rec(1, (seq_t)1, 4, 3, L);
    } else {
        /* Length >= 9.  Three possible leading digits. */
        /* Leading digit 1: zeros 4, ones 3 among the rest. */
        gen_rec(1, (seq_t)1, 4, 3, L);
        /* Leading digit 2 or 3: zeros 4, ones 4 among the rest. */
        gen_rec(1, (seq_t)2, 4, 4, L);
        gen_rec(1, (seq_t)3, 4, 4, L);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Generate enough terms to cover the requested index. */
    while (cache_len < n) {
        gen_length(next_length_to_generate);
        ++next_length_to_generate;
    }

    return cache[(size_t)n - 1];
}
