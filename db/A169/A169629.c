/*
 * A169629  Array T(n,k) read by antidiagonals: T(n,k) = Sum_{v=1..n, v odd}
 *          binomial(n,v)*k^v.
 * https://oeis.org/A169629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)-th term of the array T(n,k) read by
 *            antidiagonals, each diagonal being read from bottom (largest
 *            row) to top (smallest row). T(n,k) = Sum_{v odd, 1≤v≤n}
 *            binomial(n,v) * k^v
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* Find diagonal d such that d*(d+1)/2 ≥ n (in C‑style 1‑based count). */
static idx_t find_diagonal(idx_t n)
{
    if (n == 0) return 0;
    idx_t low  = 0;
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1, but guard anyway */
    idx_t d      = find_diagonal(n);          /* diagonal number (1‑based) */
    idx_t triPrev = (idx_t)((__int128)(d - 1) * d / 2); /* terms before diagonal d */
    idx_t offset  = n - triPrev - 1;          /* 0‑based offset inside diagonal */
    idx_t r = d - offset;                    /* row index (1‑based)   */
    idx_t k = offset + 1;                    /* column index (1‑based) */

    /* T(r,k) = ((1+k)^r - (-1)^r * (k-1)^r) / 2  */
    seq_t A = seq_pow((seq_t)(k + 1), r);
    seq_t B = seq_pow((seq_t)(k > 0 ? k - 1 : 0), r);

    seq_t result;
    if (r % 2 == 0)                     /* even r : (A - B)/2 */
        result = (A - B) / (seq_t)2;
    else                               /* odd  r : (A + B)/2 */
        result = (A + B) / (seq_t)2;

    return result;
}
