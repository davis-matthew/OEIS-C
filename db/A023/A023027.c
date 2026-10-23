/*
 * A023027  Number of partitions of n into 7 unordered relatively prime parts.
 * https://oeis.org/A023027
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * p7(n/d) * where μ is the Möbius function
 *            and p7(m) is the number of partitions * of m into exactly 7
 *            unordered parts. If m < 7 then p7(m) = 0
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* External integer types supplied by the build system */
#ifndef idx_t
typedef uint64_t idx_t;
#endif
#ifndef seq_t
/* seq_t is defined by the build; do not redefine it here. */
#endif

/* --------- static cache data --------- */
static seq_t *exact7_arr = NULL;   /* p7 values up to current max */
static idx_t exact_max = 0;        /* largest n for which exact7_arr is valid */

static int *mu_arr = NULL;         /* Möbius μ[n] up to current max */
static idx_t mu_max = 0;           /* largest n for which mu_arr is valid */

/* ---------- compute p7 up to N (inclusive) ---------- */
static void precompute_exact7(idx_t N)
{
    /* Allocate new array for p7 values */
    seq_t *new_arr = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!new_arr)
        exit(1);

    idx_t limit = (N >= 7) ? N - 7 : 0;          /* value of (m-7) when m runs 7..N */

    /* dp[i][k] stored linearly as dp[i*8 + k]   (k = 0..7) */
    seq_t *dp = (seq_t *)calloc((size_t)(limit + 1) * 8, sizeof(seq_t));
    if (!dp)
    {
        free(new_arr);
        exit(1);
    }

    /* Base case: dp[0][k] = 1 for all k */
    for (int k = 0; k <= 7; ++k)
        dp[0 * 8 + k] = 1;

    /* Build the table for all i from 1..limit */
    for (idx_t i = 1; i <= limit; ++i)
    {
        for (int k = 1; k <= 7; ++k)
        {
            seq_t val = dp[(size_t)i * 8 + (k - 1)];          /* exclude at least one k-part */
            if (i >= (idx_t)k)
                val += dp[(size_t)(i - k) * 8 + k];           /* include at least one k-part */
            dp[(size_t)i * 8 + k] = val;
        }
    }

    /* exact7_arr[m] = p7(m) for m >= 7, 0 otherwise (calloc guarantees zero) */
    if (N >= 7)
    {
        for (idx_t m = 7; m <= N; ++m)
            new_arr[m] = dp[(size_t)(m - 7) * 8 + 7];
    }

    /* Replace old array */
    free(dp);
    free(exact7_arr);
    exact7_arr = new_arr;
    exact_max = N;
}

/* ---------- compute Möbius μ up to N (inclusive) ---------- */
static void precompute_mu(idx_t N)
{
    /* Smallest prime factor array, 0 means unset */
    uint64_t *spf = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    if (!spf)
        exit(1);

    int *new_mu = (int *)calloc((size_t)(N + 1), sizeof(int));
    if (!new_mu)
    {
        free(spf);
        exit(1);
    }

    new_mu[1] = 1;           /* μ(1) = 1 */

    /* Linear sieve for spf */
    for (idx_t i = 2; i <= N; ++i)
    {
        if (spf[i] == 0)          /* i is prime */
        {
            spf[i] = i;
            if (i <= N / i)
            {
                for (idx_t j = i * i; j <= N; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* Compute μ using spf */
    for (idx_t i = 2; i <= N; ++i)
    {
        uint64_t p = spf[i];
        idx_t m = i / p;
        if (m % p == 0)
            new_mu[i] = 0;
        else
            new_mu[i] = -new_mu[(int)m];
    }

    free(spf);
    free(mu_arr);
    mu_arr = new_mu;
    mu_max = N;
}

/* ---------- ensure tables prepared up to n ---------- */
static void ensure_prepared(idx_t n)
{
    if (exact_max < n)
        precompute_exact7(n);
    if (mu_max < n)
        precompute_mu(n);
}

/* ----------------- public sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n < 7)            /* offset 7 – defined behaviour for n<7 is irrelevant */
        return 0;

    ensure_prepared(n);

    seq_t pos = 0;
    seq_t neg = 0;

    /* Enumerate divisors via sqrt loop */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            int mu1 = mu_arr[(size_t)d];
            if (mu1)
            {
                seq_t val = exact7_arr[n / d];
                if (mu1 > 0)
                    pos += val;
                else
                    neg += val;
            }
            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                int mu2 = mu_arr[(size_t)d2];
                if (mu2)
                {
                    seq_t val = exact7_arr[n / d2];
                    if (mu2 > 0)
                        pos += val;
                    else
                        neg += val;
                }
            }
        }
    }

    return pos - neg;       /* pos ≥ neg for all n */
}
