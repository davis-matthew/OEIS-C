/*
 * A067410  Triangle with columns built from certain power sequences.
 * https://oeis.org/A067410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n, m) = 1 if n == m a(n, m) = (m+2)*(2*(m+1))^(n-m-1) if n > m
 *            >= 0 Sequence a(n) is the triangle read row-wise: 1; 2,1; 4,3,1;
 *            …
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 107, u64 n <= 303,
 *            u128 n <= 787, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* fast exponentiation for seq_t */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* return the nth term of sequence A067410 */
seq_t a(idx_t n)
{
    /* find the row r such that the cumulative number of terms up to row r
       exceeds n.  The cumulative size of rows 0..r is (r+1)(r+2)/2.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column index m within the row */
    idx_t prev_terms = r * (r + 1) / 2;          /* terms before this row  */
    idx_t m = n - prev_terms;                  /* 0 ≤ m ≤ r           */

    if (m == r)                 /* last element of the row            */
        return (seq_t)1;

    idx_t e = r - m - 1;        /* exponent in the power term          */
    seq_t base = (seq_t)(2 * (m + 1));
    seq_t pow_val = power_seq(base, e);
    seq_t mult = (seq_t)(m + 2);

    return mult * pow_val;
}
