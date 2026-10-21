/*
 * A051197  T(2n+4,n), array T as in A050186; a count of aperiodic binary
 *          words.
 * https://oeis.org/A051197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | gcd(2n+4,n)} μ(d) * binomial((2n+4)/d , n/d) where
 *            μ is the Möbius function and binomial is computed
 *            multiplicatively
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* --- Helper: compute the Möbius function μ(d) for 1 ≤ d ≤ 20004 --- */
static int mu_of(idx_t d)
{
    if (d <= 1)
        return 1;                /* μ(1) = 1 */
    int sign = 1;
    idx_t n = d;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p != 0)
            continue;
        int exp = 0;
        while (n % p == 0)
        {
            n /= p;
            ++exp;
            if (exp > 1)          /* square factor → μ = 0 */
                return 0;
        }
        sign = -sign;             /* toggle sign for each distinct prime */
    }
    if (n > 1)                     /* remaining prime factor */
        sign = -sign;
    return sign;                   /* ±1 */
}

/* --- Helper: compute binomial(n,k) for 0 ≤ k ≤ n --- */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * ((seq_t)n - (seq_t)k + (seq_t)i);
        res = res / (seq_t)i;    /* exact division */
    }
    return res;
}

/* --- Main function implementing A051197 --- */
seq_t a(idx_t n)
{
    /* Compute parameters m = 2n+4 and k = n. */
    idx_t m = 2 * n + 4;
    idx_t k = n;

    /* Compute g = gcd(m, k).  We use Euclidean algorithm on idx_t. */
    idx_t g = m, temp = k;
    while (temp != 0)
    {
        idx_t r = g % temp;
        g = temp;
        temp = r;
    }

    seq_t sum = 0;

    /* Enumerate all divisors d of g. */
    for (idx_t d = 1; d * d <= g; ++d)
    {
        if (g % d != 0)
            continue;

        int mu = mu_of(d);
        if (mu != 0)
        {
            seq_t term = binom(m / d, k / d);
            if (mu > 0)
                sum += term;
            else
                sum -= term;
        }

        /* Handle the complementary divisor g/d, skip if equal to d. */
        idx_t d2 = g / d;
        if (d2 == d)
            continue;
        mu = mu_of(d2);
        if (mu != 0)
        {
            seq_t term = binom(m / d2, k / d2);
            if (mu > 0)
                sum += term;
            else
                sum -= term;
        }
    }

    return sum;
}
