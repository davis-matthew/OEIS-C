/*
 * A157756  Integers n such that by splitting their digits in two equal parts,
 *          the difference between the square of the first part and the second
 *          part eventually reaches 0 by iterating the process. If length is
 *          odd, split (length+1)/2, (length-1)/2.
 * https://oeis.org/A157756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_len, ensure_capacity, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157756: integers n such that, by splitting the decimal digits into a left part (the first ⌈d/2⌉ digits) and a right part (the remaining ⌊d/2⌋ digits), computing left² – right, and iterating this process, the value eventually reaches 0. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/ensure_capacity.h"

/* Test whether a number satisfies the A157756 property */
static bool is_good(seq_t x)
{
    seq_t cur = x;
    /* visited array to detect cycles */
    const idx_t MAX_VIS = 64;
    seq_t visited[MAX_VIS];
    idx_t vis_sz = 0;

    while (1) {
        if (cur == 0)
            return true;

        /* cycle detection */
        idx_t i;
        for (i = 0; i < vis_sz; ++i)
            if (visited[i] == cur)
                return false;

        if (vis_sz == MAX_VIS)
            return false;

        visited[vis_sz++] = cur;

        /* split the number */
        int d = digit_len(cur);          /* number of decimal digits */
        idx_t m = (idx_t)(d / 2);        /* floor(d/2) */
        seq_t powten = pow10(m);         /* 10^m */

        seq_t left  = cur / powten;
        seq_t right = cur % powten;
        seq_t left2 = left * left;

        if (left2 < right)         /* would become negative */
            return false;

        cur = left2 - right;
    }
}

/* Cached generation of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0 */
        return 0;

    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static seq_t candidate   = 0;   /* next integer to test */

    if (n <= cache_size)
        return cache[n - 1];

    while (cache_size < n) {
        if (is_good(candidate)) {
            ensure_capacity(cache_size + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = candidate;
        }
        candidate++;
    }
    return cache[n - 1];
}
