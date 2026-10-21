/*
 * A055651  Table T(m,k) = m^k - k^m (with 0^0 taken to be 1) as square array
 *          read by antidiagonals.
 * https://oeis.org/A055651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(m,k) where m = r – c, k = c and r is the smallest
 *            integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. T(m,k)=m^k – k^m
 *            (0^0 = 1)
 * Range:     i8 n <= 47, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation for seq_t values */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* position within diagonal r */
    idx_t c = n - r * (r + 1) / 2;

    /* map to table indices: m decreases from r to 0 */
    idx_t m = r - c;
    idx_t k = c;

    seq_t mk  = pow_seq((seq_t)m, k);
    seq_t km  = pow_seq((seq_t)k, m);
    return mk - km;
}
