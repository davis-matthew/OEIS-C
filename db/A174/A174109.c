/*
 * A174109  Triangle read by rows: T(n, k) = c(n, q)/(c(k, q)*c(n-k, q)),
 *          where c(n, q) = Product_{j=1..n} (j+q)!/(j-1)! and q = 8.
 * https://oeis.org/A174109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle entry T(r,k,8) read by rows. T(r,k,q) =
 *            c(r,q)/(c(k,q)*c(r-k,q)), c(n,q) = ∏_{j=1..n} (j+q)!/(j-1)! with
 *            q = 8. a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 47, u64 n <= 111, u128 n <= 308,
 *            big any n
 * Helpers:   g_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g_val.h"

/* multiply and divide using an intermediate 128‑bit type when compiling C.
   When compiling C++ the overloaded operators of seq_t produce big‑int
   arithmetic automatically. */
#if defined(__cplusplus)
#   define MUL_DIV(a,b,c) ((a) * (b) / (c))
#else
static inline seq_t mul_div_safe(seq_t a, seq_t b, seq_t c)
{
    __uint128_t prod = (__uint128_t)a * b;
    return (seq_t)(prod / c);
}
#   define MUL_DIV(a,b,c) mul_div_safe((a),(b),(c))
#endif

seq_t a(idx_t n)
{
    /* find the row r such that n lies in row r (0‑based).  A row r has
       r+1 entries, and rows are concatenated.  The cumulative count up to
       row r is r*(r+1)/2.  We locate r by binary search. */
    idx_t low = 0, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t row_start = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - row_start;               /* column index in row r  (0‑based) */
    if (k > r - k)                         /* use symmetry */
        k = r - k;

    /* compute T(r,k,8) = product_{i=1..k} g_val(r-i+1) / g_val(i)   */
    seq_t T = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = g_val(r - i + 1);
        seq_t den = g_val(i);
        T = MUL_DIV(T, num, den);
    }
    return T;
}
