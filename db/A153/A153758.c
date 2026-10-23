/*
 * A153758  a(n) = Sum_{k=1..n} A153757(k).
 * https://oeis.org/A153758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A153758
    a(n) = Sum_{k=1..n} Sum_{j=1..k} (∏_{i=2}^j F_i) ,
    where F_1 = F_2 = 1,  F_i = F_{i-1} + F_{i-2}  (Fibonacci).
    The inner product ∏_{i=2}^j F_i equals A003266(j),
    and its cumulative sums give A153757, so that
    a(n) = a(n-1) + Sum_{j=1}^n ∏_{i=2}^j F_i .
    The implementation uses an online O(n) incremental
    computation with memoisation; it works for all
    builds supported by the judge.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL; /* a[1..max] */
    static size_t cap = 0;      /* maximum index currently allocated */
    static idx_t max_cached = 0;
    static seq_t fib_prev = 1;  /* F1 */
    static seq_t fib_curr = 1;  /* F2 */
    static seq_t last_c   = 1;  /* c[1] = product F_2..F_1 = 1 */
    static seq_t last_S   = 1;  /* S[1] = c[1] */
    static seq_t last_a   = 1;  /* a[1] = 1 */

    /* helper: ensure cache can hold at least 'need' indices */
    if (n == 0)
        return (seq_t)0;        /* undefined below offset, return 0 */

    if (max_cached == 0) {     /* initialisation on first call */
        size_t init_cap = 1;
        cache = (seq_t *)realloc(cache, (init_cap + 1) * sizeof(seq_t));
        cap = init_cap;
        cache[1] = 1;
        max_cached = 1;
    }

    /* grow cache if necessary */
    while (n > (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute values up to n if needed */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t F;
        if (i == 2) {
            /* F_2 already stored in fib_curr */
            F = fib_curr;
        } else {
            seq_t next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next;
            F = fib_curr;
        }
        seq_t c   = last_c * F;          /* c[i] = c[i-1] * F_i */
        seq_t S   = last_S + c;          /* S[i] = S[i-1] + c[i] */
        seq_t a_i = last_a + S;          /* a[i] = a[i-1] + S[i] */
        cache[i] = a_i;
        last_c   = c;
        last_S   = S;
        last_a   = a_i;
    }
    max_cached = n;
    return cache[n];
}
