/*
 * A067468  Numbers n such that 7 is the first digit of 7^n.
 * https://oeis.org/A067468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m such that the first digit of
 *            7^m is 7. Computed by scanning m and keeping a running
 *            fractional part of m*log10(7)
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Constants for the interval [log10(7), log10(8)) */
static const double LOG10_7 = 0.8450980400142568;   /* log10(7) */
static const double LOG10_8 = 0.9030899879869276;   /* log10(8) */
static const double EPS = 1e-14;

/* Static storage of already computed terms */
static seq_t *terms = NULL;
static size_t cap = 0;
static size_t len = 0;
static idx_t next_exp = 1;      /* next exponent to examine */
static double frac = 0.0;       /* fractional part of (next_exp-1)*LOG10_7 */

/* Grow the terms array as needed */
static void grow_terms(size_t need)
{
    size_t newcap = cap ? cap * 2 : 16;
    if (newcap < need) newcap = need;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    terms = tmp;
    cap = newcap;
}

/* Append a new term */
static void append_term(idx_t m)
{
    if (len >= cap) grow_terms(len + 1);
    terms[len++] = (seq_t)m;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but safe */
    while (len < n)
    {
        /* Update fractional part for current exponent */
        frac += LOG10_7;
        if (frac >= 1.0) frac -= floor(frac);
        /* Check if leading digit is 7 */
        if (frac + EPS >= LOG10_7 && frac < LOG10_8) {
            append_term(next_exp);
        }
        next_exp++;
    }
    return terms[n-1];
}
