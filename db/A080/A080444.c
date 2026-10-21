/*
 * A080444  Write n A001055(n) times.
 * https://oeis.org/A080444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 631, u16 n <= 7314, u32 n <= 7314, u64 n <= 7314,
 *            u128 n <= 7314, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* A080444: a(n) is the smallest integer k such that
   Σ_{i=1..k} A001055(i) ≥ n, where A001055(i)
   is the number of multiplicative partitions (including i itself). */

/* Ensure64-bit integer square root (floor) */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t bit = (idx_t)1ULL << 62;                  /* highest power of 4 ≤ n */
    while (bit > n) bit >>= 2;
    idx_t res = 0;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Count multiplicative partitions of n with factors ≥ min_factor.
   Includes the trivial partition [n]. */
static seq_t count_mt(idx_t n, idx_t min_factor)
{
    seq_t cnt = 1;                 /* [n] itself */
    idx_t limit = int_sqrt(n);
    for (idx_t f = min_factor; f <= limit; ++f) {
        if (n % f == 0)
            cnt += count_mt(n / f, f);
    }
    return cnt;
}

/* --- caching infrastructure ------------------------------------ */
static seq_t *h   = NULL;   /* A001055 values */
static seq_t *cum = NULL;   /* cumulative sums */
static idx_t  cap  = 0;     /* allocated capacity (including index 0) */
static idx_t  last = 0;     /* highest index computed so far */

static void init_static(void)
{
    if (cap != 0) return;
    cap = 1;                     /* only index 0 initially */
    h   = seq_alloc(cap);
    cum = seq_alloc(cap);
    h[0]   = 0;
    cum[0] = 0;
    last   = 0;
}

static void ensure_capacity(idx_t needed)
{
    if (needed < cap) return;
    idx_t new_cap = cap ? cap << 1 : 128;
    while (new_cap <= needed) new_cap <<= 1;
    h   = seq_realloc(h,   cap, new_cap);
    cum = seq_realloc(cum, cap, new_cap);
    cap = new_cap;
}

static void compute_to(idx_t upto)
{
    for (idx_t i = last + 1; i <= upto; ++i) {
        seq_t val = (i == 1) ? (seq_t)1 : count_mt(i, 2); /* A001055(1)=1 */
        h[i]   = val;
        cum[i] = cum[i - 1] + val;
        last   = i;
    }
}

/* --- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    init_static();

    /* Grow cache until the cumulative sum reaches or exceeds n. */
    while (cum[last] < (seq_t)n) {
        ensure_capacity(last + 2);          /* space for next entry */
        compute_to(last + 1);               /* compute next h & cum */
    }

    /* Binary search for smallest m with cum[m] >= n. */
    idx_t low  = 1;
    idx_t high = last;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cum[mid] < (seq_t)n)
            low = mid + 1;
        else
            high = mid;
    }
    return (seq_t)low;
}
