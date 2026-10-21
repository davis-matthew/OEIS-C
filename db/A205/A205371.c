/*
 * A205371  Ordered differences of odd-indexed Fibonacci numbers.
 * https://oeis.org/A205371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{2k-1} - F_{2j-1}, where s(m)=F_{2m-1} is the m-th *
 *            odd‑indexed Fibonacci number. * k is the smallest integer with
 *            k*(k-1)/2 >= n, * j = n - (k-1)*(k-2)/2 (1 <= j < k). * * This
 *            implementation uses the supplied fib(idx_t) routine from *
 *            "../utils/fib.h" and a small integer square‑root h...
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Integer square‑root: floor(sqrt(x)) for 0 <= x < 2^64 */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > x / mid)  /* mid*mid > x  */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n >= 1; undefined otherwise */
        return 0;

    /* Find k such that k*(k-1)/2 >= n  and (k-1)*(k-2)/2 < n */
    idx_t k = (1 + isqrt_u64(1 + 8 * n)) >> 1;    /* ceil((1+sqrt(1+8n))/2) */
    while (((unsigned __int128)k * (k - 1)) / 2 < n) k++;
    while (k > 1 && (((unsigned __int128)(k - 1) * (k - 2)) / 2) >= n) k--;

    /* j is the position within the current block of k */
    idx_t prev_T = ((unsigned __int128)(k - 1) * (k - 2)) / 2;
    idx_t j = n - prev_T;   /* 1 <= j < k */

    /* Compute the odd‑indexed Fibonacci numbers */
    seq_t s_k = fib(2 * k - 1);
    seq_t s_j = fib(2 * j - 1);

    return s_k - s_j;   /* s_k > s_j for all k > j */
}
