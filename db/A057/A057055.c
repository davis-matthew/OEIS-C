/*
 * A057055  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057055(n)=i(C(n,3)).
 * https://oeis.org/A057055
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the row index i of C(n,3) in the infinite * matrix whose
 *            antidiagonals are 1; 2,3; 4,5,6; … * The element k = C(n,3) lies
 *            on diagonal d where * d(d+1)/2 ≥ k > (d−1)d/2, and its row index
 *            is * i = k − (d−1)d/2. This function returns that i
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                          /* below the offset – unused */

    /* Compute k = C(n,3) in unsigned 64‑bit arithmetic (safe for n ≤ 10 000) */
    unsigned long long n64 = n;
    unsigned long long k = n64 * (n64 - 1) * (n64 - 2) / 6;

    /* Find diagonal d: the smallest integer with d(d+1)/2 ≥ k. */
    long double val = sqrtl((long double)8.0 * (long double)k + 1.0);
    idx_t d = (idx_t)((val - 1.0L) / 2.0L);

    /* Adjust to guarantee the bound (very unlikely to be off‑by‑one). */
    while ((unsigned long long)d * (d + 1) / 2 < k)
        d++;
    while (d > 0 && (unsigned long long)(d - 1) * d / 2 >= k)
        d--;

    /* Compute the row index i = k − T(d−1), where T(m)=m(m+1)/2. */
    unsigned long long prev = (unsigned long long)d * (d - 1) / 2;
    unsigned long long i = k - prev;

    return (seq_t)i;
}
