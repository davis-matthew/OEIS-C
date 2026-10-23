/*
 * A026273  a(n) = least k such that s(k) = n, where s = A026272.
 * https://oeis.org/A026273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula used: a(n) = floor(((n-3)+(n+1)*sqrt(5))/2)  (n ≥ 1) */

#include <stdbool.h>

/* Integer floor square root using binary search on arbitrary‑precision seq_t. */
static seq_t floor_sqrt(seq_t value)
{
    seq_t low  = 0;
    seq_t high = 1;

    /* Find an upper bound for the square root. */
    while (high * high <= value)
        high <<= 1;
    low = high >> 1;

    /* Binary search to find floor(sqrt(value)). */
    while (low <= high)
    {
        seq_t mid     = low + ((high - low) >> 1);
        seq_t mid_sq  = mid * mid;

        if (mid_sq == value)
            return mid;
        if (mid_sq < value)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* OEIS sequence A026273: lower s-Wythoff sequence for s(n)=n+1. */
seq_t a(idx_t n)
{
    /* Offset is 1; return 0 for n == 0 to avoid undefined behaviour. */
    if (n == 0)
        return 0;

    /* Small values are handled directly to avoid negative intermediates. */
    if (n == 1)
        return 1;
    if (n == 2)
        return 2;

    /* For n ≥ 3 */
    seq_t k = (seq_t)n;
    k += (seq_t)1;                       /* k = n + 1 */
    seq_t t = (seq_t)5 * k * k;          /* t = 5 * (n+1)^2 */
    seq_t f = floor_sqrt(t);             /* f = floor((n+1)*sqrt(5)) */

    seq_t numer = f + (seq_t)(n - 3);     /* numer = f + (n-3)   (non‑negative) */
    return numer / 2;                    /* a(n) = floor(numer / 2) */
}
