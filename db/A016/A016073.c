/*
 * A016073  Undulating squares.
 * https://oeis.org/A016073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, nice, base
 * Formula:   a(n) = sectionally computed by a brute‑force search of squares *
 *            whose decimal expansion alternates two digits (pattern
 *            ababab...). * This implementation keeps a static cache of the
 *            known terms and extends * it by scanning successive squares
 *            until the requested index is reached
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_m = 0;      /* m for which next_m^2 will be tested */

static bool is_alternating(seq_t x)
{
    if (x == (seq_t)0)
        return true;   /* 0 is accepted */

    unsigned char digits[64];
    size_t pos = 0;

    seq_t tmp = x;
    while (tmp != (seq_t)0) {
        digits[pos++] = (unsigned char)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }

    /* reverse to most significant first */
    int even_digit = -1;
    int odd_digit  = -1;

    for (size_t j = 0; j < pos; ++j) {
        int d = digits[pos - 1 - j];
        if (j % 2 == 0) {          /* even position from left */
            if (even_digit == -1)
                even_digit = d;
            else if (even_digit != d)
                return false;
        } else {                   /* odd position from left */
            if (odd_digit == -1)
                odd_digit = d;
            else if (odd_digit != d)
                return false;
        }
    }
    return true;
}

static void ensure_cache(size_t needed)      /* we need at least needed terms (0‑based) */
{
    if (cache_size > needed)
        return;

    /* extend until we have enough terms */
    while (cache_size <= needed) {
        seq_t s = (seq_t)next_m * next_m;
        if (is_alternating(s)) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = s;
        }
        next_m++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return cache[n];
}
