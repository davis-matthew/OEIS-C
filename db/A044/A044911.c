/*
 * A044911  Numbers whose base-10 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A044911: numbers whose base‑10 run lengths alternate odd, even, odd, ... starting with odd. */
/* The routine scans integers from 1 upwards and stores each qualifying number in a cache. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool alt_run_even_odd(seq_t x)
{
    if (x == 0) return false;          /* 0 is not part of the sequence */
    unsigned char digits[200];
    size_t len = 0;
    seq_t t = x;

    /* Extract digits (least significant first) */
    while (t != 0) {
        digits[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    if (len == 0) return false;         /* safety */

    /* Handle single‑digit numbers */
    if (len == 1) return true;

    unsigned char prev = digits[len - 1];   /* most significant digit */
    size_t run_index = 0;                  /* 0‑based index of the current run */
    size_t run_len   = 1;                  /* length of current run */

    /* Traverse from second most to least significant digit */
    for (size_t off = len - 2; off < len; --off) {
        unsigned char d = digits[off];
        if (d == prev) {
            ++run_len;
        } else {
            /* Check parity alternation of the finished run */
            if ((run_len & 1) == (run_index & 1))
                return false;
            ++run_index;
            prev = d;
            run_len = 1;
        }
    }
    /* Final run check */
    if ((run_len & 1) == (run_index & 1))
        return false;

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_size = 0;
    static size_t cached_cap   = 0;
    static seq_t next_candidate = (seq_t)1;

    if (n == 0)      /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    /* Extend cache up to index n */
    while (cached_size < (size_t)n) {
        if (alt_run_even_odd(next_candidate)) {
            /* Ensure capacity */
            if (cached_size == cached_cap) {
                size_t new_cap = (cached_cap == 0) ? 256 : cached_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort(); /* Memory exhausted */
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_size++] = next_candidate;
        }
        next_candidate += (seq_t)1;
    }
    return cache[n - 1];
}
