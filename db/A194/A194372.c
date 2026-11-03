/*
 * A194372  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(3) and < > denotes fractional part.
 * https://oeis.org/A194372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m such that the number of k ≤ m with the fractional part of
 *            k*sqrt(3) less than 1/2 equals m/2 (i.e. the sum ∑(<1/2 +
 *            k*sqrt(3)> − <k*sqrt(3)>) is zero)
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 962 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *vals = NULL;          /* stored sequence values */
static size_t vals_len = 0;         /* how many are stored */
static size_t vals_cap = 0;         /* capacity of the array */
static idx_t next_m = 0;            /* next m that has not been examined */
static seq_t count_lt = 0;          /* count of k with frac(k*sqrt(3)) < 1/2 */

/* Append a value to the dynamic array. */
static void append_val(seq_t v)
{
    if (vals_len == vals_cap) {
        size_t new_cap = vals_cap ? vals_cap * 2 : 8;
        vals = mem_resize(seq_t, vals, new_cap);
        vals_cap = new_cap;
    }
    vals[vals_len++] = v;
}

/* Return the nth term (1‑based) of A194372.  Caches computed terms
   so that any call order is supported. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for indices < 1 */
        return (seq_t)0;

    /* Extend precomputation until we have at least n terms. */
    while (vals_len < (size_t)n) {
        next_m++;                         /* examine this m */
        seq_t m = (seq_t)next_m;

        /* A(m) = floor(m * sqrt(3)) = floor(sqrt(3 * m^2)) */
        seq_t prod  = (seq_t)3 * m * m;   /* 3 * m^2  */
        seq_t A     = floor_sqrt(prod);

        /* Determine whether fractional part of m*sqrt(3) is < 1/2. */
        /* Compare 12*m^2 and (2*A+1)^2. */
        seq_t lhs   = (seq_t)12 * m * m;
        seq_t rhs   = ((seq_t)2 * A + 1) * ((seq_t)2 * A + 1);
        bool frac_lt_half = lhs < rhs;   /* true  ⇔  frac < 1/2 */

        if (frac_lt_half)
            count_lt = count_lt + 1;

        /* m must be even for 2*count_lt == m. */
        if (((m & 1) == 0) && (count_lt * 2 == m))
            append_val(m);
    }

    return vals[n - 1];
}
