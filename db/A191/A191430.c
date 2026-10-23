/*
 * A191430  Dispersion of ([n*sqrt(2)+3/2]), where [ ]=floor, by
 *          antidiagonals.
 * https://oeis.org/A191430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A191430: array rows are the dispersion of
 * floor(n*sqrt(2)+3/2).  The sequence a(n) is the antidiagonal
 * read of that array.  The implementation generates the complement
 * sequence t(i) on demand and applies the Beatty mapping s(k)
 * defined below. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Integer square root of a 128‑bit unsigned integer (O(log bits)) */
static inline idx_t floor_sqrt_u128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64; /* sqrt(v) < 2^64 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)lo;
}

/* Beatty mapping for s(k) = floor(k*sqrt(2)+3/2) */
static inline seq_t s_func(idx_t k)
{
    /* compute floor(k*sqrt(2)) as x = floor_sqrt(2*k^2)   */
    unsigned __int128 prod = (unsigned __int128)2 * k * k;
    idx_t x = floor_sqrt_u128(prod);

    /* decide whether to add 1 or 2 :  floor(k*sqrt(2)+3/2) = x+1 or x+2   */
    unsigned __int128 lhs = (unsigned __int128)8 * k * k;
    unsigned __int128 rhs = (unsigned __int128)4 * x * x + (unsigned __int128)4 * x + 1;
    seq_t add = (lhs >= rhs) ? 2 : 1;

    return (seq_t)(x + add);
}

/* --- false if n is NOT in the Beatty sequence s(k) --- */
static bool in_s(idx_t n)
{
    if (n == 0) return false;
    idx_t low = 1, high = 1;
    /* find an upper bound high such that s(high) >= n */
    while (s_func(high) < n) {
        if (high > ((idx_t)-1) >> 1) {  /* prevent overflow */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }
    /* binary search for equality */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t val = s_func(mid);
        if (val == n) return true;
        if (val < n) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

/* --- dynamic array for the complement t(i) --- */
static seq_t *t_vals = NULL;
static idx_t t_len = 0;
static idx_t t_cap = 0;
static idx_t next_candidate = 1;   /* next integer to test for membership */

static void push_t(seq_t v)
{
    if (t_len == t_cap) {
        idx_t new_cap = t_cap ? t_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(t_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        t_vals = tmp;
        t_cap = new_cap;
    }
    t_vals[t_len++] = v;
}

/* Ensure t_vals has at least r_needed entries (1‑based). */
static void ensure_t_len(idx_t r_needed)
{
    while (t_len < r_needed) {
        if (!in_s(next_candidate))
            push_t((seq_t)next_candidate);
        next_candidate++;
    }
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    /* find diagonal index d: smallest d with (d+1)(d+2)/2 >= n */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < n) ++d;

    /* index within diagonal (0‑based) */
    idx_t prev_terms = d * (d + 1) / 2;
    idx_t pos = n - prev_terms - 1;          /* zero based */
    idx_t r = pos + 1;          /* row number (1‑based) */
    idx_t c = d - pos;          /* number of s applications */

    ensure_t_len(r);
    seq_t result = t_vals[r - 1];

    for (idx_t i = 0; i < c; ++i)
        result = s_func((idx_t)result);

    return result;
}
