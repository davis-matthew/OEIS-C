/*
 * A000275  Coefficients of a Bessel function (reciprocal of J_0(z)); also
 *          pairs of permutations with rise/rise forbidden.
 * https://oeis.org/A000275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{r=0..n-1} (-1)^(r+n+1) binomial(n, r)^2 a(r), a(0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A275_MAX 20001

static seq_t a275_memo[A275_MAX];
static idx_t a275_count = 0;

seq_t a(idx_t n)
{
    idx_t m, r;
    if (n >= A275_MAX)
        n = A275_MAX - 1;
    if (a275_count == 0) {
        a275_memo[0] = 1;
        a275_count = 1;
    }
    for (m = a275_count; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        seq_t c = 1; /* binomial(m, r) */
        for (r = 0; r < m; r++) {
            seq_t t = c * c * a275_memo[r];
            if (((r + m + 1) & 1) == 0)
                pos += t;
            else
                neg += t;
            c = c * (seq_t)(m - r) / (seq_t)(r + 1);
        }
        a275_memo[m] = pos - neg;
        a275_count = m + 1;
    }
    return a275_memo[n];
}
