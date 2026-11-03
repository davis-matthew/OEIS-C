/*
 * A197774  Suppose n has prime factorization n = p1^a1 * p2^a2 * ... * pk^ak.
 *          Then a(n) = (-1)^(n1 + n2 + ... + nk) if all the ai are ni^2 and
 *          a(n) = 0 otherwise.
 * https://oeis.org/A197774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e | n} (-1)^{√e} if every exponent e is a perfect
 *            square, otherwise 0. The product is over the prime power
 *            factorisation of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9796 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int is_square_exp(int exp, int *sqrt_out)
{
    int r = 0;
    while ((idx_t)r * r < exp) r++;
    if ((idx_t)r * r == exp) {
        if (sqrt_out) *sqrt_out = r;
        return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t nn = n;
    int parity = 0;

    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            int r;
            if (!is_square_exp(exp, &r)) return (seq_t)0;
            parity ^= (r & 1);
        }
    }

    if (nn > 1) {          /* remaining prime factor with exponent 1 */
        int r = 1;
        parity ^= (r & 1);
    }

    return (seq_t)(parity ? -1 : 1);
}
