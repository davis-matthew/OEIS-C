/*
 * A228297  Generalized meta-Fibonacci sequence a(n) with parameters s=0 and
 *          k=5.
 * https://oeis.org/A228297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer m such that * ∑_{i=1}^{m} (v5(i)+1)
 *            ≥ n, where v5(i) is the exponent of 5 in i
 * Range:     u8 n <= 318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v5_of_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/v5_of_integer.h"

static idx_t *cumul   = NULL;   /* cumulative counts, size len */
static idx_t *vals    = NULL;   /* corresponding integer m, size len */
static size_t  len    = 0;
static size_t  cap    = 0;
static idx_t   last_m = 0;
static idx_t   last_cum = 0;

/* grow the arrays when needed */
static void
grow_arrays(void)
{
    size_t new_cap = (cap == 0) ? 4 : cap * 2;
    idx_t *new_cumul = (idx_t *)realloc(cumul, new_cap * sizeof(idx_t));
    idx_t *new_vals  = (idx_t *)realloc(vals,  new_cap * sizeof(idx_t));
    if (!new_cumul || !new_vals) {
        abort();
    }
    cumul = new_cumul;
    vals  = new_vals;
    cap   = new_cap;
}

/* extend cache up to at least n */
static void
ensure_upto(idx_t n)
{
    while (last_cum < n) {
        idx_t m = last_m + 1;
        idx_t repeats = v5_of_integer(m) + 1;
        idx_t new_cum = last_cum + repeats;

        if (len == cap) {
            grow_arrays();
        }

        vals[len] = m;
        cumul[len] = new_cum;
        len++;
        last_m = m;
        last_cum = new_cum;
    }
}

/* binary search to find index of first cumulative >= n */
static size_t
find_index(idx_t n)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cumul[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    ensure_upto(n);
    size_t idx = find_index(n);
    return (seq_t)vals[idx];
}
