/*
 * A085833  Start at (2n+1)/4 and iterate the map x -> x*ceiling(x); sequence
 *          gives values of n for which the denominators in the orbit go from
 *          4 to 2, instead of dropping directly to 1.
 * https://oeis.org/A085833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the nth integer n such that in the orbit of
 *            (2n+1)/4 under the map x -> x*ceil(x) the denominator drops from
 *            4 to 2 before reaching 1. The sequence is computed by
 *            brute‑force enumeration
 * Range:     u8 n <= 2, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* local gcd for the generic unsigned type seq_t */
static seq_t gcd_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* test whether a particular n satisfies the property */
static int qualifies(idx_t n)
{
    seq_t num = (seq_t)(2 * n + 1); /* numerator (odd) */
    seq_t den = (seq_t)4;           /* initial denominator */

    /* loop until the denominator drops to 1, 2, or a loop with ceil==1 is detected */
    for (int step = 0; step < 128; ++step) {
        if (den == 1) return 0;      /* dropped directly to 1 */
        if (den == 2) return 1;      /* reached 2 before 1 */

        /* compute ceil(num/den) = (num + den - 1) / den */
        seq_t ceil_val = (num + den - 1) / den;
        if (ceil_val == 1) return 0; /* fraction stays the same, denom never reduces */

        /* new numerator after multiplying by ceil_val */
        seq_t new_num = num * ceil_val;
        seq_t g = gcd_local(new_num, den);
        num = new_num / g;
        den = den / g;
    }
    /* if the loop limit is hit, we treat it as not qualifying */
    return 0;
}

/* static storage for the sequence values */
static seq_t *seq  = NULL;
static size_t  len  = 0;
static size_t  cap  = 0;
static idx_t   next_candidate = 1;

/* helper to grow the dynamic array */
static void grow_seq(void)
{
    if (cap == 0) {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    } else {
        cap <<= 1;
        seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    }
}

/* retrieve the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; for safety */

    /* generate terms until we have at least n elements */
    while (len < (size_t)n) {
        while (!qualifies(next_candidate))
            ++next_candidate;

        if (len == cap)
            grow_seq();

        seq[len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return seq[n - 1];
}
