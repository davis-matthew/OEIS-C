/*
 * A078821  Triangular array T given by T(n,k) = number of 01-words of length
 *          n having exactly k 1's and all runlengths odd.
 * https://oeis.org/A078821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(T(n,k)), where * T(n,k) is the number of 01‑words
 *            of length n having exactly k 1’s * and all run lengths odd. * *
 *            T(n,k) = Σ_{r≡n (mod 2)} Σ_{b∈{0,1}} * C(X1+m-1, m-1) *
 *            C(X0+(r-m)-1, (r-m)-1) * * m = (r+b)/2 (number of 1‑runs) * S =
 *            (n-r)/2 * diff = k-m (must be even) * X1 = diff/...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* compute binomial coefficient C(n,k) for arbitrary seq_t */
static seq_t binomial_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g   = gcd(num, den);
        num /= g;
        den /= g;
        res /= den;
        res *= num;
    }
    return res;
}

/* number of 01‑words of length n having exactly k ones and all run lengths odd */
static seq_t T_value(idx_t n, idx_t k)
{
    if (n == 0) return 0;

    seq_t total = 0;
    idx_t r_start = (n & 1) ? 1 : 2;          /* r has same parity as n, r ≥ 1 */
    for (idx_t r = r_start; r <= n; r += 2) {
        seq_t S = (seq_t)(n - r) / 2;
        for (idx_t b = 0; b < 2; ++b) {
            /* m = number of 1‑runs for this start bit */
            idx_t m = (r + b) / 2;
            if (k < m) continue;

            idx_t diff = k - m;
            if ((diff & 1) != 0) continue;    /* k-m must be even */

            idx_t X1_i = diff / 2;            /* extra pairs of 1’s */
            seq_t X1   = (seq_t)X1_i;

            if (X1 > S) continue;            /* cannot use more than S pairs */

            seq_t X0 = S - X1;                /* extra pairs of 0’s */

            /* reject impossible configurations with no runs of a type */
            if ((r - m) == 0 && X0 != 0) continue;
            if (m == 0 && X1 != 0) continue;

            seq_t ways1 = 1;
            seq_t ways0 = 1;
            if (m > 0) {
                ways1 = binomial_seq( (idx_t)X1 + m - 1, m - 1 );
            }
            if (r - m > 0) {
                ways0 = binomial_seq( (idx_t)X0 + (r - m) - 1, (r - m) - 1 );
            }
            total += ways1 * ways0;
        }
    }
    return total;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( ((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n ) {
        ++r;
    }
    idx_t col = n - r * (r + 1) / 2;
    return T_value(r, col);
}
