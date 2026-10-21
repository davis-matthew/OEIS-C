/*
 * A094263  Triangle read by rows: for 1 <= k <= n, a(n, k) = n^k mod k.
 * https://oeis.org/A094263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, less
 * Formula:   a(n) is the nth number of the triangle with entries a(n,k) = n^k
 *            mod k (row n, column k). The sequence is read row‑by‑row; a(n)
 *            is the element whose linear index is n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine computes the index (row, column) for a given n,
    then evaluates n^k mod k using binary exponentiation.  */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 128‑bit numbers using binary search.
   Returns floor(sqrt(x)). */
static inline idx_t
isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0, hi = (idx_t)1;
    while ((unsigned __int128)hi * hi <= x) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* Compute r such that r(r+1)/2 >= n and minimal. */
static inline idx_t
row_of(idx_t n)
{
    unsigned __int128 v = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(v);
    idx_t r = (s >= 1) ? (s - 1) >> 1 : 0;

    /* Adjust for possible rounding errors. */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && ((unsigned __int128)(r - 1) * r / 2) >= n) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; value unused for n=0 */

    idx_t r = row_of(n);                     /* row number */
    idx_t prev_sum = (unsigned __int128)(r - 1) * r / 2; /* terms before row r */
    idx_t k = n - prev_sum;                  /* column number (1‑based) */

    /* Modulo k: if k==1 the result is always 0. */
    seq_t mod = (seq_t)k;
    if (mod == 1) return 0;

    seq_t base = (seq_t)r;
    seq_t result = 1 % mod;
    idx_t exp = k;

    while (exp) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
