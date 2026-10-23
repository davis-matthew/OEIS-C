/*
 * A192000  Sum of binomial numbers A000332(k+3), with k in the reduced
 *          residue system modulo n.
 * https://oeis.org/A192000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n-1, gcd(k,n)=1} binomial(k+3,4)
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small integer Euclidean GCD (n ≤ 10 000, fits in int). */
static inline int int_gcd(int a, int b)
{
    while (b)
    {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        if (int_gcd((int)k, (int)n) == 1)
        {
            uint64_t kk = k;
            uint64_t term = kk * (kk + 1) * (kk + 2) * (kk + 3) / 24;
            sum += term;
        }
    }
    return (seq_t)sum;
}
