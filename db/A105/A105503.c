/*
 * A105503  Numbers n such that 3 is the leading digit of the n-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105503: indices n such that the leading digit of the n‑th
   Fibonacci number F_n is 3.
   The function computes the n‑th such index by scanning successive
   Fibonacci indices and testing their leading digit via logarithms.
   The test uses high‑precision long double arithmetic to avoid
   overflow and to produce exact leading digits for the required
   range of indices. */
#include <stdlib.h>
#include <math.h>

static seq_t *list = NULL;          // array of found indices
static idx_t list_size = 0;         // number of elements stored
static idx_t list_cap  = 0;         // allocated capacity
static idx_t last_k = 1;            // next Fibonacci index to check

/* Leading‑digit constants for long double arithmetic */
static const long double LOG10_PHI   = 0.20898764024997873L;
static const long double LOG10_SQRT5 = 0.3494850021680094L;

/* Ensure enough capacity in the array. */
static void ensure_cap(idx_t needed)
{
    if (needed <= list_cap) return;
    idx_t new_cap = list_cap ? list_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    list = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
    list_cap = new_cap;
}

/* Return true if the leading digit of Fibonacci number F_k is 3. */
static inline int leading_is_three(idx_t k)
{
    long double exponent = k * LOG10_PHI - LOG10_SQRT5;
    long double frac     = exponent - floorl(exponent);
    long double leading  = powl(10.0L, frac);
    int digit = (int)(leading + 1e-12L);   /* small epsilon for rounding */
    return digit == 3;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe */
    while (list_size < n)
    {
        if (leading_is_three(last_k))
        {
            ensure_cap(list_size + 1);
            list[list_size++] = (seq_t)last_k;
        }
        ++last_k;
    }
    return list[n - 1];
}
