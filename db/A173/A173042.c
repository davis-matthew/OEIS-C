/*
 * A173042  Numbers n that cannot be decomposed into the sum of up to 4
 *          squares using the following algorithm: If n is not decomposable
 *          using the algorithm: [Repeat the following 2 steps 4 times: 1-find
 *          the largest square s smaller than n; 2-n=n-s Numbers that can be
 *          decomposed yield final values of n=0.] th...
 * https://oeis.org/A173042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A173042 – numbers that cannot be decomposed into the sum of up
    up to 4 squares using the greedy algorithm described in the OEIS
    entry.  A(n) is given by the first n numbers that fail both the
    largest‑square greedy search and the second‑largest‑square variant. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Greedy decomposition: at each step subtract the largest square
   LESS THAN OR EQUAL TO the current value.  Return true if the
   routine reaches zero within at most 'steps' steps. */
static bool greedy_decompose(seq_t m, int steps)
{
    while (steps-- && m > 0) {
        seq_t k = floor_sqrt(m);          /* k^2 <= m < (k+1)^2 */
        if (k == 0) return false;        /* cannot reduce further */
        m -= k * k;
    }
    return m == 0;
}

/* Return true if n cannot be decomposed by either greedy method. */
static bool is_non_decomposable(seq_t n)
{
    /* First try: largest square <= n, up to 4 squares. */
    if (greedy_decompose(n, 4))
        return false;

    /* Second try: first square is the second largest square <= n. */
    seq_t k = floor_sqrt(n);              /* largest square root */
    /* If n is a perfect square, k^2 == n; otherwise k^2 < n. */
    seq_t s2 = (k - 1) * (k - 1);         /* second largest square <= n */
    if (s2 == 0)                          /* n < 1, shouldn't happen */
        return true;

    seq_t remaining = n - s2;
    return !greedy_decompose(remaining, 3);
}

/* Exported sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = 1;                         /* start from 1 */

    while (true) {
        if (is_non_decomposable(num)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
