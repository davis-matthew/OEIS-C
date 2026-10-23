/*
 * A044510  Numbers n such that string 4,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044510: numbers n such that the base‑6 representation contains the substring
   "43" but the base‑6 representation of n+1 does not.  The for‑each call cache
   stores the found terms so that repeated queries are answered in O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool has_43(seq_t n)
{
    unsigned char digits[128];
    int len = 0;
    seq_t t = n;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            digits[len++] = (unsigned char)(t % 6);
            t /= 6;
        }
    }

    for (int i = 0; i + 1 < len; ++i) {
        if (digits[i] == 3 && digits[i + 1] == 4)
            return true;
    }
    return false;
}

static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 128 : (*cap) * 2;
    if (newcap < needed)
        newcap = needed;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap = 0;
    static seq_t next_val = 0;

    if (n == 0)
        return (seq_t)0;

    while (cache_used < n) {
        ensure_capacity(cache_used + 1, &cache_cap, &cache);

        if (has_43(next_val) && !has_43(next_val + 1))
            cache[cache_used++] = next_val;
        ++next_val;
    }

    return cache[n - 1];
}
