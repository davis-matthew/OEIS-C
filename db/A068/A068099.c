/*
 * A068099  a(1) = 1; a(n+1) = n! * Sum_{d|n} a(d)/d where the sum is over the
 *          positive divisors d of n.
 * https://oeis.org/A068099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n-1} a(d) / d where a(1) = 1 and a(n) is
 *            defined for n ≥ 2 via the recurrence above
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state for memoisation */
static seq_t *a_arr = NULL;      /* array of computed terms (1‑based)          */
static size_t a_len = 0;          /* current capacity of a_arr                 */
static size_t a_calculated = 0;   /* largest index with a_arr[i] computed      */
static seq_t fact = (seq_t)1;     /* (a_calculated)!, initialized to 1!        */

/* Ensure that a_arr can hold at least indices 0..n */
static void ensure_capacity(size_t n)
{
    if (a_len < n + 1) {
        size_t new_len = a_len ? a_len * 2 : 2;
        if (new_len < n + 1) new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        a_arr = tmp;
        a_len = new_len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Index 1 is defined; if n == 0, we return 0 (undefined). */
    if (n == 0) return (seq_t)0;
    size_t idx = (size_t)n;

    /* Initialise on first call */
    if (a_calculated == 0) {
        ensure_capacity(1);
        a_arr[0] = (seq_t)0;            /* unused */
        a_arr[1] = (seq_t)1;            /* a(1) = 1 */
        a_calculated = 1;
        fact = (seq_t)1;                /* 1! */
    }

    /* Expand memoisation up to idx */
    ensure_capacity(idx);
    for (size_t i = a_calculated + 1; i <= idx; ++i) {
        size_t m = i - 1;               /* m = i-1, factorial currently m! */
        seq_t sum = (seq_t)0;

        /* Enumerate all divisors of m */
        for (size_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                size_t d1 = d;
                seq_t term1 = a_arr[d1] * (fact / (seq_t)d1);
                sum += term1;

                size_t d2 = m / d;
                if (d2 != d1) {
                    seq_t term2 = a_arr[d2] * (fact / (seq_t)d2);
                    sum += term2;
                }
            }
        }

        a_arr[i] = sum;                  /* store a(i) */
        /* Update factorial to (i)! for next iteration */
        fact *= (seq_t)i;
        a_calculated = i;
    }

    return a_arr[idx];
}
