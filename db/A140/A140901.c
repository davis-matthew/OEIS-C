/*
 * A140901  Number of 3 X 5 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 3,5,n can be permuted, see
 *          formula.
 * https://oeis.org/A140901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{3} ∏_{j=1}^{5} (i + j + n - 1)/(i + j - 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Empty product is 1, so a(0)=1).  Each factor is a rational number; the
   final product is an integer.  The implementation multiplies the
   numerator–denominator pairs into a prime‑exponent map, then evaluates
   the integer result from the exponents.  The algorithm runs in
   O(1) time for fixed matrix size (3×5) and is independent of the
   maximal index n. */

#include <stdint.h>

static void add_factor(idx_t val, int sign,
                       idx_t primes[2048], int exp[2048], int *cnt)
{
    if (val <= 1) return;               /* 0 or 1 have no prime factors   */
    idx_t x = val;
    for (idx_t d = 2; d * d <= x; ++d)
    {
        if (x % d == 0)
        {
            int c = 0;
            while (x % d == 0)
            {
                x /= d;
                ++c;
            }
            int i = 0;
            while (i < *cnt && primes[i] != d) ++i;
            if (i == *cnt)
            {
                primes[*cnt] = d;
                exp[*cnt] = 0;
                ++(*cnt);
                i = *cnt - 1;
            }
            exp[i] += sign * c;
        }
    }
    if (x > 1)   /* remaining prime factor */
    {
        idx_t d = x;
        int i = 0;
        while (i < *cnt && primes[i] != d) ++i;
        if (i == *cnt)
        {
            primes[*cnt] = d;
            exp[*cnt] = 0;
            ++(*cnt);
            i = *cnt - 1;
        }
        exp[i] += sign;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* empty product */
        return (seq_t)1;

    idx_t primes[2048];
    int   exp[2048];
    int   cnt = 0;

    /* For each i,j add the numerator (t+n-1) and denominator (t-1) */
    for (idx_t i = 1; i <= 3; ++i)
    {
        for (idx_t j = 1; j <= 5; ++j)
        {
            idx_t t = i + j;           /* sum ranges 2..8               */
            idx_t numer = t + n - 1;   /* (i+j+n-1)                     */
            idx_t denom = t - 1;       /* (i+j-1)                       */

            add_factor(numer, +1, primes, exp, &cnt);
            add_factor(denom, -1, primes, exp, &cnt);
        }
    }

    /* Evaluate the product from the prime exponents */
    seq_t result = (seq_t)1;
    for (int k = 0; k < cnt; ++k)
    {
        if (exp[k] > 0)
        {
            /* Compute prime^exp[k] by repeated multiplication */
            seq_t power = (seq_t)1;
            for (int e = 0; e < exp[k]; ++e)
                power *= (seq_t)primes[k];
            result *= power;
        }
    }
    return result;
}
