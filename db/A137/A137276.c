/*
 * A137276  Triangle T(n,k), read by rows: T(n,k)= 0 if n-k odd. T(n,k)=
 *          2*(-1)^((n-k)/2)*(2k-n)/(n+k)*binomial((n+k)/2,(n-k)/2) if n-k
 *          even.
 * https://oeis.org/A137276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 56, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A137276 – triangle read by rows.
 *  T(n,k) = 0 if n-k is odd;
 *  otherwise T(n,k)= 2·(-1)^((n-k)/2)·(2k-n)/(n+k)·binomial((n+k)/2,(n-k)/2).
 *  The linear sequence a(n) is obtained by enumerating the triangle by rows:
 *  a(n)=T(r,c) where r,c satisfy r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *  c = n - r(r+1)/2.
 *
 *  The implementation uses the provided binomial routine:
 *      seq_t binomial(idx_t n, idx_t k);
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for the very first element (T(0,0)=1). */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Find the row r such that offset(r) ≤ n < offset(r+1),
     * where offset(r) = r*(r+1)/2.  We use a simple linear increase
     * of the upper bound, which is O(log n) because r grows like
     * sqrt(2n) and the subsequent binary search is O(log r). */
    idx_t low = 0;
    idx_t high = n;          /* start with arbitrary bound ≥ r */
    while (((unsigned __int128)high * (high + 1) / 2) <= n) {
        ++high;
    }

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* upper mid */
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = low;

    /* Compute column index c in the row. */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - offset;   /* 0 ≤ c ≤ r */

    /* If n-k is odd, the entry is 0. */
    idx_t diff = r - c;
    if (diff & 1ULL) {
        return (seq_t)0;
    }

    /* Even difference: evaluate the closed form. */
    idx_t k = diff / 2;                     /* k = (n-k)/2  */
    idx_t m = (r + c) / 2;                  /* m = (n+k)/2  */

    seq_t bin = binomial(m, k);

    /* numerator: 2 * bin * (2c - r) */
    seq_t diff_dc = (seq_t)2 * (seq_t)c - (seq_t)r;   /* 2c - r  */
    seq_t num = (seq_t)2 * bin * diff_dc;

    /* denominator: r + c  (never zero for n>0) */
    seq_t denom = (seq_t)(r + c);

    seq_t val = num / denom;                 /* exact division */

    /* Apply the sign (-1)^k. */
    if (k & 1ULL) {
        val = -val;
    }

    return val;
}
