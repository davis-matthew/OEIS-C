/*
 * A105513  Number of times 3 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k : 0 ≤ k ≤ n and the leading decimal digit of
 *            Fibonacci number F_k is 3}
 * Range:     u8 n <= 2037, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the property that the leading digit of F_k is determined by the fractional part
   of k*log10(φ) – 0.5*log10(5).  It keeps a prefix‑sum table so repeated calls are fast. */

#include <math.h>
#include <stdlib.h>

/* Precomputed constants for the logarithms needed. */
static const long double LOG10_PHI = log10l((1.0l + sqrtl(5.0l)) / 2.0l);
static const long double LOG10_5_2  = log10l(5.0l) / 2.0l;
static const long double LOG10_3   = log10l(3.0l);
static const long double LOG10_4   = log10l(4.0l);

/* Returns 1 if the leading decimal digit of Fibonacci number F_k is 3, 0 otherwise. */
static inline int is_leading_three(idx_t k)
{
    if (k == 0) return 0; /* F_0 = 0 has no leading digit 3 */
    long double y = (long double)k * LOG10_PHI - LOG10_5_2;
    long double frac = y - floorl(y);
    return (frac >= LOG10_3 && frac < LOG10_4);
}

/* Prefix–sum array storing a(k) for 0 ≤ k ≤ current_max.  It is grown on demand. */
static seq_t *pref   = NULL; /* pref[i] = a(i) */
static idx_t pref_len = 0;    /* number of stored entries (size of pref array) */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have entries up to n. */
    if (pref_len == 0) {
        /* Allocate first entry for k = 0. */
        pref = (seq_t *)malloc(sizeof(seq_t));
        pref[0] = (seq_t)0;
        pref_len = 1;
    }

    if (n >= pref_len) {
        /* Grow the array to hold indices up to n. */
        seq_t *newpref = (seq_t *)realloc(pref, (size_t)(n + 1) * sizeof(seq_t));
        if (!newpref) { /* allocation failure: return zero as a safe fallback */
            return (seq_t)0;
        }
        pref = newpref;

        /* Compute entries beyond the previous maximum. */
        for (idx_t k = pref_len; k <= n; ++k) {
            seq_t val = pref[k - 1];
            if (is_leading_three(k)) ++val;
            pref[k] = val;
        }
        pref_len = n + 1;
    }

    return pref[n];
}
