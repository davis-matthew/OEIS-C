/*
 * A166682  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
 * A166682(t) = (t^12 + 2*t^11 + 2*t^10 + 2*t^9 + 2*t^8 + 2*t^7 + 2*t^6
 *                 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1)
 *              / (528*t^12 - 32*t^11 - 32*t^10 - 32*t^9 - 32*t^8
 *                 - 32*t^7 - 32*t^6 - 32*t^5 - 32*t^4 - 32*t^3
 *                 - 32*t^2 - 32*t + 1).
 *
 * Let b(n) = 1 if n = 0, 2 if 1 ≤ n ≤ 11, 1 if n = 12, 0 otherwise.
 * Then for all n ≥ 0:
 *
 *     a(n) = b(n) + 32*(a(n-1)+a(n-2)+…+a(n-11)) - 528*a(n-12)
 *
 * (Terms a(k) for k < 0 are taken as 0.)  This produces the sequence
 * with a(0)=1, a(1)=34, a(2)=1122, … exactly as in OEIS A166682.  The
 * function below computes any a(n) on demand, memoising previously
 * computed terms.  It works for any integer type seq_t (unsigned or
 * arbitrary‑precision) and index type idx_t = uint64_t. */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Allocate or grow memoisation cache as needed.  The cast below
       is required for strict C++ compilation; it is harmless in C. */
    static seq_t *cache = NULL;
    static idx_t cur_max = (idx_t)-1;   /* no values computed yet */

    size_t needed = (size_t)n + 1;
    if (!cache) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
    } else if (needed > (size_t)(cur_max + 1)) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t val;                          /* b(i) */
        if (i == 0)           val = 1;
        else if (i <= 11)     val = 2;
        else if (i == 12)     val = 1;
        else                  val = 0;

        seq_t sum = val;

        /* Add 32 * a(i-j) for j = 1..11, if available. */
        for (int j = 1; j <= 11; ++j) {
            if (i >= (idx_t)j) {
                sum += cache[i - j] * (seq_t)32;
            }
        }
        /* Subtract 528 * a(i-12) if available. */
        if (i >= 12) {
            sum -= cache[i - 12] * (seq_t)528;
        }

        cache[i] = sum;
    }

    cur_max = n;
    return cache[n];
}
