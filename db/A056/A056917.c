/*
 * A056917  Numbers n such that sigma(n)-phi(n) is a perfect square.
 * https://oeis.org/A056917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th integer m such that sigma(m) - phi(m) is a perfect
 *            square
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 884 (2026-11-03)
 * Helpers:   ipow_i32, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a lazy sieve: maintain a dynamic array of found values
   and extend it until the requested index is reached.  The helper functions
   from "utils" are used for sigma, phi and integer square root. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

/* dynamic array to cache the sequence values */
static seq_t *seq = NULL;   /* array of seq_t */
static idx_t  seq_len = 0;  /* number of values currently stored */
static idx_t  seq_cap = 0;  /* allocated capacity */

/* ensure that the array can hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t newcap = seq_cap > 0 ? seq_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq = mem_resize(seq_t, seq, newcap);
    seq_cap = newcap;
}

/* append a new value to the cached array */
static void append_value(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq[seq_len++] = val;
}

/* test whether n satisfies the defining property */
static bool property_holds(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t phi   = totient_seq(n);
    seq_t diff  = sigma - phi;
    seq_t root  = floor_sqrt(diff);
    return (root * root) == diff;
}

/* public accessor for the n-th element of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1, return zero safely */

    static idx_t curr = 1; /* next integer to test */

    while (seq_len < n) {
        if (property_holds(curr))
            append_value(curr);
        ++curr;
    }
    return seq[n - 1];
}
