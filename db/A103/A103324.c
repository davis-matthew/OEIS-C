/*
 * A103324  Square array T(n,k) read by antidiagonals: powers of Lucas
 *          numbers.
 * https://oeis.org/A103324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = lucas(k)^((s+1)-k), where s = the unique integer
 *            satisfying s*(s+1)/2 < n <= (s+1)*(s+2)/2, k = n - s*(s+1)/2 -
 *            1. (k is the column index, n-1 is the row index)
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   lucas, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lucas.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n >= 1; the offset is 1.  For n == 0 we return 0 (unused). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Find s such that (s+1)(s+2)/2 >= n.  Since n <= 10^4 in the
       intended tests, a simple linear scan is fast enough. */
    idx_t s = 0;
    while (((__int128)(s + 1) * (s + 2) / 2) < n) {
        ++s;
    }

    /* previous cumulative count */
    idx_t prev = s * (s + 1) / 2;     /* T(s-1)  */
    idx_t pos  = n - prev - 1;        /* 0‑based position in the antidiagonal */

    idx_t k   = pos;                  /* column index */
    idx_t exp = (s + 1) - k;          /* exponent (row index) */

    seq_t base = lucas(k);            /* Lucas number L(k) */
    return pow_seq(base, exp);        /* L(k) ^ exp */
}
