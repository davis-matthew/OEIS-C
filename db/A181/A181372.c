/*
 * A181372  Square array read by antidiagonals: a(p,n) is the number of
 *          inversions in all p-ary words of length n on {0,1,2,...,p-1}
 *          (p>=2, n>=2).
 * https://oeis.org/A181372
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n(n-1)(p-1)p^{n-1} / 4, where (p,n) is obtained from the
 *            antidiagonal ordering of the square array with p>=2, n>=2. For
 *            the OEIS indexing (offset 2) the first term is a(2,2)=1
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local power function: computes base^exp for integer exp (idx_t) */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1ULL;
        if (exp)
            base *= base;
    }
    return result;
}

/* Computes a(idx) for idx >= 2.
   Maps idx to the pair (p,n) according to the antidiagonal order. */
seq_t a(idx_t idx)
{
    if (idx < 2)
        return (seq_t)0;           /* undefined for the offset area */

    /* zero‑based position inside the flattened sequence */
    idx_t idx0 = idx - 2;

    /* binary search for the smallest m such that m(m+1)/2 > idx0 */
    uint64_t lo = 1, hi = ((uint64_t)1) << 34;   /* upper bound > possible m */
    while (lo < hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t > idx0)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t m = lo;                       /* m = s - 3 where s = p + n */

    /* position inside the current antidiagonal */
    unsigned __int128 prev_sum = (unsigned __int128)(m - 1) * m / 2;
    idx_t pos = idx0 - (idx_t)prev_sum;    /* 0 <= pos < m */

    /* Recover (p,n) */
    idx_t n_val = pos + 2;                 /* n >= 2 */
    idx_t p_val = (m + 1) - pos;           /* p >= 2 */

    /* Compute the formula n(n-1)(p-1)p^{n-1} / 4 in seq_t */
    seq_t n = (seq_t)n_val;
    seq_t p = (seq_t)p_val;

    seq_t result = n * (n - 1);            /* n(n-1)   */
    result *= (p - 1);                     /* n(n-1)(p-1) */
    result *= pow_seq(p, n_val - 1);       /* * p^{n-1} */
    result /= 4;                           /* /4        */

    return result;
}
