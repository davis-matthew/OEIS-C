/*
 * A117878  Triangle T(n,k) = A034386(n)*A049614(k) - 1 read by rows.
 * https://oeis.org/A117878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = A034386(r)*A049614(k) - 1, where r and k are obtained by
 *            reading the triangular array T(n,k) = A034386(n)*A049614(k) - 1
 *            row‑wise. r = smallest integer with r*(r+1)/2 ≥ n, k = n -
 *            r*(r-1)/2
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 89, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* integer square root (floor) for 64‑bit unsigned integers */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = x;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* primality test by trial division – sufficient for n ≤ 10000 */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    for (idx_t i = 2; (unsigned long long)i * i <= n; ++i)
        if (n % i == 0)
            return false;
    return true;
}

/* product of all primes ≤ n  (A034386) */
static seq_t primorial(idx_t n)
{
    seq_t acc = (seq_t)1;
    for (idx_t p = 2; p <= n; ++p)
        if (is_prime(p))
            acc = acc * (seq_t)p;
    return acc;
}

/* product of all composite numbers ≤ k  (A049614) */
static seq_t invprimorial(idx_t k)
{
    seq_t acc = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i)
        if (!is_prime(i))
            acc = acc * (seq_t)i;
    return acc;
}

/* row index r for the n‑th element in the row‑wise reading of the triangle */
static idx_t row_of(idx_t n)
{
    idx_t r = (isqrt_u64(8 * (unsigned long long)n + 1ULL) - 1ULL) / 2ULL;
    while (r * (r + 1ULL) / 2ULL < n)
        ++r;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t k = n - r * (r - 1ULL) / 2ULL;

    seq_t p = primorial(r);
    seq_t s = invprimorial(k);

    return p * s - (seq_t)1;
}
