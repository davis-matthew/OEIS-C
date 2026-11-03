/*
 * A225884  Triangular numbers whose binary and decimal reversals are also
 *          triangular numbers.
 * https://oeis.org/A225884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the n‑th triangular number T_k * such that
 *            reverse_bin(T_k) and reverse_dec(T_k) are also triangular
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   ipow_i32, mem, reverse_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/reverse_bits.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Generation of consecutive triangular numbers */
static idx_t curr_k = 0;    /* current k with T_k = curr_tri */
static seq_t curr_tri = 0;  /* current triangular number T_k */

/* Test whether x is a triangular number: 8x+1 must be a perfect square with odd root. */
static bool is_triangular(seq_t x)
{
    seq_t s = x * 8 + 1;
    seq_t r = floor_sqrt(s);
    return (r * r == s) && (r & 1);
}

/* Reverse decimal digits of x. */
static seq_t reverse_decimal(seq_t x)
{
    seq_t res = 0;
    seq_t t = x;
    while (t != 0)
    {
        res = res * 10 + (t % 10);
        t /= 10;
    }
    return res;
}

/* Grow cache array to at least 'needed' capacity. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (a(1) = 0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined below it */
    size_t wanted = (size_t)n;
    if (cache_len < wanted)
    {
        grow_cache(wanted);
        while (cache_len < wanted)
        {
            seq_t t = curr_tri;
            if (is_triangular(reverse_bits(t)) &&
                is_triangular(reverse_decimal(t)))
            {
                cache[cache_len++] = t;
            }
            curr_k += 1;
            curr_tri += (seq_t)curr_k;
        }
    }
    return cache[n - 1];
}
