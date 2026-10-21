/*
 * A096123  Least product n*(n-1)*(n-2)*...*(n-k+1) divisible by (n-k)!.
 * https://oeis.org/A096123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest product n*(n-1)*…*(n-k+1) divisible by
 *            (n-k)!. This is equivalent to a(n) = n! / t! where t is the
 *            largest integer ≤ n–1 such that (t!)² divides n!
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers (do not include any of the provided utils). */
#include <stdbool.h>
#include <stdlib.h>

/* Helper: exponent of prime p in m! (for m, p of type idx_t). */
static inline seq_t vp_factorial(idx_t m, idx_t p)
{
    seq_t exp = 0;
    while (m)
    {
        m /= p;
        exp += (seq_t)m;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1 by definition */
        return (seq_t)1;

    /* ------------- Build sieve of primes up to n ------------- */
    /* Allocate boolean array size n+1 (indices 0..n). */
    bool *isPrime = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!isPrime)           /* allocation failed */
        return (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
        isPrime[i] = true;

    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (isPrime[i])
        {
            for (idx_t j = i * i; j <= n; j += i)
                isPrime[j] = false;
        }
    }

    /* ------------- Find minimal t (largest allowed) ------------- */
    idx_t t = n - 1;        /* start with maximal possible value */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (!isPrime[p]) continue;   /* skip composites */

        seq_t vp_n = vp_factorial(n, p);
        seq_t bound = vp_n / 2;     /* floor division */

        /* Binary search for the largest t_p with vp(t_p!) <= bound */
        idx_t low = 0, high = n - 1, best = 0;
        while (low <= high)
        {
            idx_t mid = low + ((high - low) >> 1);
            seq_t vp_mid = vp_factorial(mid, p);

            if (vp_mid <= bound)
            {
                best = mid;
                low = mid + 1;
            }
            else
            {
                if (mid == 0) break;  /* avoid underflow */
                high = mid - 1;
            }
        }

        if (best < t)
            t = best;
    }

    /* ------------- Compute the product n! / t! = (t+1)*(t+2)*…*n ------------- */
    seq_t result = (seq_t)1;
    for (idx_t i = t + 1; i <= n; ++i)
        result *= (seq_t)i;

    free(isPrime);
    return result;
}
