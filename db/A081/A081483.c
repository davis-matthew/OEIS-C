/*
 * A081483  Consider the mapping f(a/b) = (a^2 + b)/(a^2 - b). Taking a =2, b
 *          = 1 to start with and carrying out this mapping repeatedly on each
 *          new (reduced) rational number gives the following sequence
 *          2/1,5/3,14/11,207/185,... Sequence contains the numerator.
 * https://oeis.org/A081483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A081483.
   It iteratively applies the mapping
         f(a/b) = (a² + b) / (a² – b)
   to a reduced rational number, starting from 2/1.
   The sequence consists of the numerators of the reduced fractions.
   The function memoises terms so that arbitrary order of calls is fast. */

#include <stddef.h>
#include <stdlib.h>

/* Assume seq_t and idx_t are defined by the build environment. */

/* Local Euclidean gcd for arbitrary-precision seq_t values. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Memoised storage of numerators and denominators. */
static seq_t *numerators = NULL;
static seq_t *denominators = NULL;
static size_t memo_size = 0;       /* number of computed terms */
static size_t memo_cap  = 0;       /* allocated capacity */

/* Ensure the memo tables have space for at least 'need' entries. */
static void ensure_capacity(size_t need)
{
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_nums = (seq_t *)realloc(numerators, new_cap * sizeof(seq_t));
    seq_t *new_dens = (seq_t *)realloc(denominators, new_cap * sizeof(seq_t));
    if (!new_nums || !new_dens) exit(1);
    numerators = new_nums;
    denominators = new_dens;
    memo_cap = new_cap;
}

/* Compute a(n).  The function is safe for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but return 0 safely */

    /* Extend memoisation to at least n terms. */
    while (memo_size < (size_t)n)
    {
        if (memo_size == 0)
        {
            /* initialise first term: 2/1 */
            ensure_capacity(1);
            numerators[0] = (seq_t)2;
            denominators[0] = (seq_t)1;
            memo_size = 1;
            continue;
        }

        seq_t a_prev = numerators[memo_size - 1];
        seq_t d_prev = denominators[memo_size - 1];

        seq_t a_sq = a_prev * a_prev;        /* a_prev² */
        seq_t num = a_sq + d_prev;           /* numerator before reduction */
        seq_t den = a_sq - d_prev;           /* denominator before reduction */

        seq_t g = gcd_seq_t(num, den);       /* common divisor */
        if (g != (seq_t)1)
        {
            num = num / g;
            den = den / g;
        }

        /* store result */
        ensure_capacity(memo_size + 1);
        numerators[memo_size] = num;
        denominators[memo_size] = den;
        memo_size++;
    }

    return numerators[n - 1];
}
