/*
 * A127881  Integers of the form x^5/120 + x^4/24 + x^3/6 + x^2/2 + x + 1 with
 *          x > 0.
 * https://oeis.org/A127881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(x) where f(x) = x^5/120 + x^4/24 + x^3/6 + x^2/2 + x +
 *            1 and x is the nth positive integer for which f(x) is an integer
 *            (OEIS A127881, offset 1)
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 774, u128 n <= 5000,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 4771 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the polynomial value for a given x */
static seq_t value_for_x(idx_t x)
{
    unsigned __int128 xm = (unsigned __int128)x;
    unsigned __int128 numerator =
        xm * xm * xm * xm * xm          /* x^5  */
        + 5 * xm * xm * xm * xm         /* 5x^4 */
        + 20 * xm * xm * xm              /* 20x^3 */
        + 60 * xm * xm                   /* 60x^2 */
        + 120 * xm                       /* 120x */
        + 120;                           /* +120 */
    return (seq_t)(numerator / 120);
}

/* Cache for memoisation of computed values */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_x = 1;          /* next x to test */

/* Which residues modulo 120 make the numerator divisible by 120 */
static bool good_residue[120] = {0};
static bool residues_computed = false;

/* Pre‑compute the good residues modulo 120 */
static void compute_good_residues(void)
{
    if (residues_computed)
        return;
    for (int r = 0; r < 120; ++r) {
        idx_t rr = (idx_t)r;
        idx_t r2 = (rr * rr) % 120;
        idx_t r3 = (r2 * rr) % 120;
        idx_t r4 = (r3 * rr) % 120;
        idx_t r5 = (r4 * rr) % 120;
        idx_t n_mod = (r5 + (5 * r4) % 120 + (20 * r3) % 120 + (60 * r2) % 120) % 120;
        good_residue[r] = (n_mod == 0);
    }
    residues_computed = true;
}

/* Grow the cache if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *ptr = mem_resize(seq_t, seq_cache, new_cap);
    if (!ptr)
        abort();          /* out of memory */
    seq_cache = ptr;
    cache_cap = new_cap;
}

/* Requested function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1 */

    compute_good_residues();

    while (cache_len < (size_t)n) {
        /* find next x with integer f(x) */
        while (!good_residue[(size_t)(next_x % 120)]) {
            ++next_x;
        }
        seq_t val = value_for_x(next_x);
        ensure_cache_capacity();
        seq_cache[cache_len++] = val;
        ++next_x;
    }
    return seq_cache[n - 1];
}
