/*
 * A097602  a(n+1) = a(n) + number of squares so far; a(1) = 1.
 * https://oeis.org/A097602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=a(n)+# of squares among a(1)..a(n)
 * Range:     u8 n <= 21, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8938 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

/* Dynamic cache of sequence values (1‑based). */
static seq_t *seq   = NULL;   /* seq[0] holds a(1) */
static size_t seq_len = 0;    /* number of terms currently stored */
static size_t seq_cap = 0;    /* allocated capacity of seq */
static size_t sq_cnt  = 0;    /* number of square terms seen */

/* Ensure the array has space for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq = mem_resize(seq_t, seq, new_cap);
    seq_cap = new_cap;
}

/* Return the n-th term (1‑based).  Values are memoised and can be
   obtained in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1; return 0 for safety */

    /* Grow the cache up to index n. */
    while (seq_len < (size_t)n) {
        ensure_capacity(seq_len + 1);

        if (seq_len == 0) {          /* initialise first term */
            seq[0] = (seq_t)1;
            sq_cnt  = 1;              /* 1 is a perfect square */
            seq_len = 1;
            continue;
        }

        /* Compute next term: a(k+1)=a(k)+sq_cnt. */
        seq_t next = seq[seq_len - 1] + (seq_t)sq_cnt;
        seq[seq_len] = next;
        seq_len++;

        /* Test whether 'next' is a perfect square. */
        seq_t r = floor_sqrt(next);
        if (r * r == next)
            sq_cnt++;
    }

    return seq[n - 1];
}
