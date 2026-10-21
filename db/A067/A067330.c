/*
 * A067330  Triangle read by rows of incomplete convolutions of Fibonacci
 *          numbers F(n+1) = A000045(n+1), n>=0.
 * https://oeis.org/A067330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 202, u32 n <= 854, u64 n <= 3714,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A067330: a(n) = sum_{k=0}^m F(k+1)*F(r-k+1)
   where r = floor((sqrt(8n+1)-1)/2) is the row number,
   m = n - r(r+1)/2 is the column number in the triangle.
   F denotes Fibonacci numbers with F(0)=0, F(1)=1. */
/* Computes the flattened triangle of incomplete convolutions of Fibonacci numbers. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: binary search for the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until exceeding n */
    while (high * (high + 1) / 2 <= n) {
        high <<= 1;            /* double until greater */
    }
    /* Binary search on [low, high) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t m = n - r * (r + 1) / 2;

    /* Need Fibonacci numbers up to F(r+1) */
    idx_t fib_lim = r + 1;
    seq_t *fib = (seq_t *)malloc((fib_lim + 1) * sizeof(seq_t));
    if (!fib)
        return 0;   /* Out of memory: return 0 as fallback */

    fib[0] = 0;
    if (fib_lim >= 1)
        fib[1] = 1;
    for (idx_t i = 2; i <= fib_lim; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    seq_t result = 0;
    for (idx_t k = 0; k <= m; ++k) {
        result += fib[k + 1] * fib[r - k + 1];
    }

    free(fib);
    return result;
}
