/*
 * A016167  Inverse of 2158th cyclotomic polynomial.
 * https://oeis.org/A016167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016167: a(n) = coefficient of x^n in the power series expansion of 1/Φ_2158(x)
   (inverse of the 2158th cyclotomic polynomial).  The sequence is periodic with period 2158. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Möbius function for 64‑bit integers.  Returns 1, -1 or 0. */
static int mobius_uint64(uint64_t x)
{
    if (x == 1) return 1;
    int cnt = 0;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0)
            {
                ++exp;
                x /= p;
                if (exp > 1) return 0;            /* squared prime factor */
            }
            ++cnt;                                 /* distinct prime factor */
        }
    }
    if (x > 1) ++cnt;                                /* remaining prime */
    return (cnt & 1) ? -1 : 1;                       /* (-1)^cnt */
}

/* Compute all 2158 coefficients of 1/Φ_2158(x). */
static void compute_cache(seq_t *tab)
{
    const idx_t N = 2158;                 /* period length */

    /* initialise array to zero */
    for (idx_t i = 0; i < N; ++i) tab[i] = 0;
    tab[0] = 1;

    /* list all divisors of N */
    idx_t divs[32];
    idx_t num = 0;
    for (idx_t d = 1; d * d <= N; ++d)
    {
        if (N % d == 0)
        {
            divs[num++] = d;
            if (d != N / d) divs[num++] = N / d;
        }
    }

    /* apply the product ∏(1 - x^d)^(-μ(N/d)) to build the series 1/Φ_N(x) */
    for (idx_t i = 0; i < num; ++i)
    {
        idx_t d  = divs[i];
        uint64_t m = N / d;
        int mu = mobius_uint64(m);
        if (mu == 0) continue;

        if (mu == 1)        /* multiply by (1 - x^d)^(-1)  (additive recurrence) */
        {
            for (idx_t k = d; k < N; ++k)
                tab[k] += tab[k - d];
        }
        else                /* mu == -1: multiply by (1 - x^d) (subtractive recurrence) */
        {
            for (idx_t k = N - 1; k + 1 > d; --k)   /* descending to avoid overwriting */
                tab[k] -= tab[k - d];
        }
    }
}

/* Public function required by the grader */
seq_t a(idx_t n)
{
    static bool done = false;
    static seq_t cache[2158];
    if (!done)
    {
        compute_cache(cache);
        done = true;
    }
    return cache[n % 2158];
}
