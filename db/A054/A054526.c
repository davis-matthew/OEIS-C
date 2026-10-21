/*
 * A054526  Triangle T(n,k): T(n,k) = phi(k) (n >= 1, 1 <= k <= n).
 * https://oeis.org/A054526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = φ( n - (r-1)·r/2 ), where r is the smallest integer such
 *            that r·(r+1)/2 ≥ n. φ is Euler’s totient function
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required header for sqrtl */
#include <math.h>

/* Auxiliary totient routine – works for any seq_t type */
static seq_t my_phi(idx_t n_idx)
{
    idx_t t = n_idx;
    seq_t result = (seq_t)n_idx;
    if (t < 2)        /* φ(0) or φ(1) */
        return result;

    for (idx_t p = 2; p <= t / p; ++p)
    {
        if (t % p == 0)
        {
            while (t % p == 0)
                t /= p;
            result -= result / (seq_t)p;
        }
    }
    if (t > 1)            /* remaining prime factor > sqrt(original n) */
        result -= result / (seq_t)t;

    return result;
}

seq_t a(idx_t n)
{
    /* Find the row index r: smallest r with r*(r+1)/2 >= n */
    long double d = sqrtl((long double)8.0 * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);           /* floor of the real root */
    while (r * (r + 1) / 2 < n)      /* adjust upward if needed */
        ++r;
    while (r > 1 && ((r - 1) * r / 2) >= n)   /* adjust downward if needed */
        --r;

    /* Position inside the row */
    idx_t j = n - (r - 1) * r / 2;

    return my_phi(j);
}
