/*
 * A084556  n occurs n! times.
 * https://oeis.org/A084556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest m such that sum_{k=1}^{m} k! ≥ n, with
 *            a(0)=0 (i.e. each integer m occurs m! times in the sequence)
 * Range:     u8 n <= 9450, u16 n <= 9450, u32 n <= 9450, u64 n <= 9450,
 *            u128 n <= 9450, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for UINT64_MAX */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return 0;

    idx_t target = n;   /* 1‑based index into the sequence starting after a(0) */
    idx_t m = 0;        /* current integer whose factorial is being accumulated */
    idx_t fact = 1;     /* factorial of m (m! ) */
    idx_t sum = 0;      /* cumulative sum Σ_{k=1}^{m} k! */

    /* Iterate until the cumulative sum reaches or exceeds the target index. */
    while (1)
    {
        ++m;

        /* Multiply fact by m, saturating on overflow. */
        if (fact > UINT64_MAX / m)
            fact = UINT64_MAX;
        else
            fact *= m;

        /* Add fact to sum, saturating on overflow. */
        if (sum > UINT64_MAX - fact)
            sum = UINT64_MAX;
        else
            sum += fact;

        if (sum >= target)
            break;
    }

    return (seq_t)m;
}
