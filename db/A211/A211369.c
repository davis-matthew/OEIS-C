/*
 * A211369  Array read by antidiagonals: T(m,n) = m*(m+n-1)! + Sum( n <= i <=
 *          m+n-2 ) i!
 * https://oeis.org/A211369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m, n') where * d = smallest integer with d(d+1)/2 ≥ n *
 *            m = n − d(d−1)/2 ( 1 ≤ m ≤ d ) * n' = d − m + 1 ( 1 ≤ n' ≤ d ) *
 *            T(m,n') = m*(m+n'−1)! + Σ_{i=n'}^{m+n'−2} i!
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 74, u64 n <= 197,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_diagonal(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;          /* d ≤ n for n ≥ 1 */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1)) >> 1 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* not part of the sequence, safe fallback */

    /* locate diagonal d and indices m , n' */
    idx_t d = find_diagonal(n);
    unsigned __int128 prev = ((unsigned __int128)(d - 1) * d) >> 1;
    idx_t m  = (idx_t)(n - prev);
    idx_t n_prime = d - m + 1;

    /* compute factorials up to (d‑1)! and accumulate the required sum */
    seq_t sum = (seq_t)0;
    seq_t fact = (seq_t)1;        /* fact = i! for current i */

    for (idx_t i = 1; i <= d - 1; ++i) {
        fact = fact * (seq_t)i;         /* now fact = i! */
        if (i >= n_prime)
            sum = sum + fact;           /* add to Σ_{i=n'}^{d-1} i! */
    }

    /* fact now holds (d‑1)!, so factorial of d is fact * d */
    seq_t fact_d = fact * (seq_t)d;

    /* result = m * d! + Σ_{i=n'}^{d-1} i!  (note m*(m+n'−1)! = m*d!) */
    seq_t result = (seq_t)m * fact_d + sum;
    return result;
}
