/*
 * A061266  Number of squares between n^3 and (n+1)^3.
 * https://oeis.org/A061266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n = 2, otherwise * a(n) = floor(sqrt((n+1)^3 - 1)) -
 *            floor(sqrt(n^3 + 1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 98 terms through n = 1802 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Handle the special case n = 2 */
    if (n == (idx_t)2)
        return (seq_t)3;

    /* Compute the required squares using 64–bit intermediates to avoid overflow. */
    unsigned long long n_ul  = (unsigned long long)n;
    unsigned long long n_cube  = n_ul * n_ul * n_ul;          /* n^3           */
    unsigned long long n1     = n_ul + 1;
    unsigned long long n1_cube = n1 * n1 * n1;                /* (n+1)^3       */

    /* Apply the integer square root routine on the truncated values. */
    seq_t high = isqrt_seq((seq_t)(n1_cube - 1));   /* floor sqrt((n+1)^3 - 1) */
    seq_t low  = isqrt_seq((seq_t)(n_cube + 1));    /* floor sqrt(n^3 + 1)     */

    return high - low;
}
