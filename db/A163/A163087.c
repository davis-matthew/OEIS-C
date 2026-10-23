/*
 * A163087  Product{k|n} k$. Here '$' denotes the swinging factorial function
 *          (A056040).
 * https://oeis.org/A163087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163087
   a(n) = ∏_{d|n} swing(d),
   where swing(d) = 1                 for d ≤ 1
   swing(d) = binom(d, d/2)           for d  even
   swing(d) = ((d+1)/2) * binom(d, (d-1)/2)  for d  odd   */

/* Binomial coefficient C(n,k) computed multiplicatively.
   It uses seq_gcd from "../utils/seq_gcd.h" to cancel factors. */
#include "../utils/seq_gcd.h"

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;            /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = seq_gcd(num, den);
        num /= g;  den /= g;

        g = seq_gcd(res, den);
        res /= g;  den /= g;

        res *= num; /* den must be 1 now */
    }
    return res;
}

/* Swinging factorial */
static seq_t swing(idx_t n)
{
    if (n <= 1) return 1;
    if ((n & 1) == 0) {            /* even */
        return binom(n, n / 2);
    } else {                       /* odd */
        seq_t part = (seq_t)((n + 1) / 2);
        return part * binom(n, (n - 1) / 2);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* offset 0 */

    seq_t result = 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            result *= swing(d);
            idx_t other = n / d;
            if (other != d) result *= swing(other);
        }
    }
    return result;
}
