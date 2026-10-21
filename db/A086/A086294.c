/*
 * A086294  Sum of distinct prime factors of 7-smooth numbers.
 * https://oeis.org/A086294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of the nth 7‑smooth number
 *            (primes ≤ 7). The sequence is 0,2,3,2,5,5,7,2,3,7,5, ... This
 *            implementation generates the 7‑smooth numbers using the classic
 *            “ugly‑number” dynamic programming method. For each generated
 *            number we keep a mask of which of the primes 2...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *smooth_vals = NULL;   /* generated 7‑smooth numbers    */
static uint8_t  *smooth_mask = NULL;   /* bitmask of {2,3,5,7} presence */
static idx_t    smooth_len  = 0;       /* number of generated elements   */
static idx_t    smooth_cap  = 0;       /* capacity of the arrays         */
static idx_t    i2 = 0, i3 = 0, i5 = 0, i7 = 0;  /* indices for each prime      */

static void ensure_capacity(idx_t new_len)
{
    if (new_len <= smooth_cap) return;
    idx_t new_cap = smooth_cap > 0 ? smooth_cap * 2 : 16;
    while (new_cap < new_len) new_cap *= 2;
    smooth_vals  = (uint64_t *)realloc(smooth_vals, new_cap * sizeof *smooth_vals);
    smooth_mask  = (uint8_t  *)realloc(smooth_mask, new_cap * sizeof *smooth_mask);
    smooth_cap   = new_cap;
}

/* Generate n 7‑smooth numbers if we do not have them yet */
static void generate_upto(idx_t n)
{
    for (; smooth_len < n; ++smooth_len)
    {
        if (smooth_len == 0)
        {
            smooth_vals[0] = 1ULL;          /* first 7‑smooth number */
            smooth_mask[0] = 0;             /* no prime factors */
            continue;
        }

        uint64_t cand2 = smooth_vals[i2] * 2ULL;
        uint64_t cand3 = smooth_vals[i3] * 3ULL;
        uint64_t cand5 = smooth_vals[i5] * 5ULL;
        uint64_t cand7 = smooth_vals[i7] * 7ULL;

        uint64_t minv = cand2;
        if (cand3 < minv) minv = cand3;
        if (cand5 < minv) minv = cand5;
        if (cand7 < minv) minv = cand7;

        uint8_t    minmask = 0;
        if (minv == cand2) { i2++; minmask |= 0x1; }
        if (minv == cand3) { i3++; minmask |= 0x2; }
        if (minv == cand5) { i5++; minmask |= 0x4; }
        if (minv == cand7) { i7++; minmask |= 0x8; }

        smooth_vals[smooth_len] = minv;
        smooth_mask[smooth_len] = minmask;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1 but harmless */
    ensure_capacity(n);
    generate_upto(n);

    uint8_t mask = smooth_mask[n - 1];
    seq_t sum = (seq_t)((mask & 0x1) ? 2 : 0)
              + (seq_t)((mask & 0x2) ? 3 : 0)
              + (seq_t)((mask & 0x4) ? 5 : 0)
              + (seq_t)((mask & 0x8) ? 7 : 0);
    return sum;
}
