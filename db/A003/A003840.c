/*
 * A003840  Order of (usually) simple Chevalley group D_n (9).
 * https://oeis.org/A003840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n*(n-1)) * (9^n - 1) * Product_{i=1..n-1} (9^(2*i) -
 *            1) / gcd(4, 9^n - 1), and gcd(4, 9^n - 1) = 4 for all n >= 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t q = 9;
    seq_t qn = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        qn *= q;
    /* 9^n - 1 is divisible by 8, so divide by 4 exactly first */
    seq_t r = (qn - 1) / 4;

    seq_t q2 = 81;
    seq_t p = 1;
    for (i = 1; i < n; i++) {
        p *= q2;
        r *= (p - 1);
    }

    /* multiply by 9^(n*(n-1)) = 81^(n*(n-1)/2) */
    idx_t e = n * (n - 1) / 2;
    seq_t base = q2;
    seq_t pw = 1;
    while (e > 0) {
        if (e & 1)
            pw *= base;
        e >>= 1;
        if (e > 0)
            base *= base;
    }
    r *= pw;
    return r;
}
