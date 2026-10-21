/*
 * A084346  Triangle read by rows in which row n gives decomposition of
 *          Fib(n)*Fib(n+1) into non-adjacent Fibonacci numbers (given by
 *          their indices).
 * https://oeis.org/A084346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) gives the index of the Fibonacci number in the Zeckendorf *
 *            decomposition of Fib(n)*Fib(n+1). The decomposition is read row
 *            by row * in a lower‑triangular array
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t x)
{
    /* integer square root of an unsigned 64‑bit integer, implemented
     * by binary search.  The product mid*mid is performed in unsigned
     * __int128 to avoid overflow. */
    idx_t lo = 0, hi = (idx_t)1 << 32;  /* hi^2 > 2^64-1 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n >= 1; safety fallback */

    /* Find the row r that contains the position n. */
    idx_t k = isqrt_u64(n - 1);                     /* floor(sqrt(n-1)) */
    idx_t r;
    unsigned __int128 k_mul = (unsigned __int128)k * (k + 1);
    if (k_mul >= n)
        r = 2 * k;
    else
        r = 2 * k + 1;

    /* Length of row r and number of terms before the row. */
    idx_t len;
    unsigned __int128 prev_total;
    if (r & 1) {                 /* r is odd */
        idx_t k_odd = (r - 1) / 2;
        len = k_odd + 1;
        prev_total = (unsigned __int128)k_odd * (k_odd + 1);
    } else {                     /* r is even */
        idx_t k_even = r / 2;
        len = k_even;
        prev_total = (unsigned __int128)k_even * k_even;
    }

    /* Offset inside the row (0‑based). */
    idx_t offset = n - (idx_t)prev_total - 1;

    /* First (left‑most) index of the row. */
    unsigned __int128 first = (unsigned __int128)(2 * r - 1);

    /* Compute the required index. */
    unsigned __int128 result;
    if (r & 1) {                 /* odd row: last term is 2 */
        if (offset + 1 == len)
            result = 2;
        else
            result = first - (unsigned __int128)4 * offset;
    } else {                     /* even row: all terms differ by 4 */
        result = first - (unsigned __int128)4 * offset;
    }

    return (seq_t)result;
}
