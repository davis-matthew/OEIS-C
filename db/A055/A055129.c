/*
 * A055129  Repunits in different bases: table by antidiagonals of numbers
 *          written in base k as a string of n 1's.
 * https://oeis.org/A055129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) is the value of the repunit T(N,K) = * T(N,K) = 1 + K + K^2
 *            + … + K^(N-1). * The pairs (N,K) are taken in anti‑diagonal
 *            order * of the table T(n,k) with n≥1, k≥1 (T(n,1)=n). * For k>1,
 *            T(n,k) = (k^n-1)/(k-1)
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 182, u64 n <= 453,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Return T(N,K) = (k^n-1)/(k-1) (or n if k==1) using a simple recurrence. */
static seq_t
repunit_value(idx_t N, idx_t K)
{
    if (K == 1)                      /* special case, T(n,1)=n */
        return (seq_t)N;

    seq_t val = 1;                    /* T(1,K) = 1 */
    for (idx_t i = 1; i < N; ++i)
        val = K * val + 1;            /* T(i+1,K) = K*T(i,K)+1 */

    return val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* defined for safety */

    /* Find t = S-1 such that t*(t+1)/2 >= n  where S is the current anti‑diagonal sum. */
    long double dn = (long double)n;
    long double d  = sqrtl(8.0L * dn + 1.0L);
    idx_t t = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct t to satisfy t*(t+1)/2 ≥ n and (t-1)*t/2 < n. */
    while (t * (t + 1) / 2 < n)
        ++t;
    while (t > 0 && (t - 1) * t / 2 >= n)
        --t;

    idx_t sum_prev = (t - 1) * t / 2;   /* indices before current anti‑diagonal */
    idx_t offset   = n - sum_prev;      /* 1 … t  (position inside the diagonal) */

    idx_t N = offset;                   /* number of 1's */
    idx_t K = (t + 1) - offset;         /* base */

    return repunit_value(N, K);
}
