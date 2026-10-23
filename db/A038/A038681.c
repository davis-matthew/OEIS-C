/*
 * A038681  Fifth powers ending nontrivially in a nonzero fifth power.
 * https://oeis.org/A038681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   digit_len, ensure_capacity, pow10, pow_seq, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of all fifth powers n^5 whose decimal
   representation ends, in a non‑trivial way, in a non‑zero fifth power.
   The implementation searches successive integers m, computes m^5,
   and checks all proper suffixes.  A binary‑search root test is used
   to determine whether a given suffix is a fifth power.  Found terms
   are cached so that arbitrary calling order is supported. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_seq.h"
#include "../utils/pow_uint.h"
#include "../utils/ensure_capacity.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* ----------  Helper: test if a seq_t value is a non‑zero fifth power ----------
   Returns true if x = k^5 for some integer k>0.  Uses binary search on the
   integer 5th root.  This routine works for arbitrary‑precision seq_t. */
static bool is_fifth_power_big(seq_t x)
{
    if (x == 0) return false;
    if (x == 1) return true;        /* 1 = 1^5 */

    /* Compute an upper bound for the fifth root: 10^((digits+4)/5) */
    unsigned char d = digit_len(x);
    seq_t lo = 1;
    seq_t hi = pow10((idx_t)((d + 4) / 5));

    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t mid5 = pow_seq(mid, 5);

        if (mid5 == x) return true;
        if (mid5 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

/* ----------  Helper: does m^5 have a proper suffix that is a fifth power? ----------
   Returns true if there exists a k with 1 ≤ k < digits(m^5) such that
   the last k digits of m^5 form a non‑zero fifth power. */
static bool has_good_suffix(idx_t m)
{
    seq_t m5 = pow_seq((seq_t)m, 5);
    seq_t mod = 10;            /* 10^1 */

    /* Iterate over suffix lengths from 1 up to digits(m5)-1 */
    while (mod <= m5) {
        seq_t suffix = m5 % mod;
        if (suffix > 0 && is_fifth_power_big(suffix))
            return true;
        mod *= 10;              /* advance to next power of ten */
    }
    return false;
}

/* ----------  Main sequence function ----------
   Returns a(n) for n ≥ 1.  Indices start at 1 (adjacent to OEIS offset). */
seq_t a(idx_t n)
{
    /* Static cache to store already found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_m      = 1;   /* next integer to test */

    /* Build cache until it contains the nth term. */
    while (cache_len < n) {
        /* Search for the next integer whose fifth power ends nicely. */
        while (!has_good_suffix(next_m))
            ++next_m;

        /* Compute the term and store it. */
        seq_t term = pow_seq((seq_t)next_m, 5);

        if (cache_len == cache_cap)
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void**)&cache, sizeof(seq_t));
        cache[cache_len++] = term;
        ++next_m;       /* next search will start from following integer */
    }
    /* n is 1‑based. */
    return cache[n - 1];
}
