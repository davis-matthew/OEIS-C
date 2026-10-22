/*
 * A004333  Binomial coefficient C(4n,n-3).
 * https://oeis.org/A004333
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-3) = Product_{i=1..n-3} (3*n+3+i)/i
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   a004333_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004333_gcd.h"

seq_t a(idx_t n)
{
    idx_t k = n - 3;
    seq_t base = (seq_t)3 * (seq_t)n + 3;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004333_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
