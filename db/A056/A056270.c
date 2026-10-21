/*
 * A056270  Number of primitive (aperiodic) words of length n which contain
 *          exactly five different symbols.
 * https://oeis.org/A056270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d)·A001118(n/d) where A001118(k) = #5‑ary words
 *            of length k that use all 5 symbols
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------------- generic power for seq_t -------------------------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* ----------------- binomial coefficients for 5 choose j --------------------- */
static const seq_t C5[6] = {
    (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)10, (seq_t)5, (seq_t)1
};

/* ----------------- A001118(k) ------------------------------------------- */
static seq_t A001118(idx_t k)
{
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;
    for (int j = 0; j <= 5; ++j)
    {
        seq_t term = pow_seq((seq_t)(5 - j), k);
        term = term * C5[j];
        if ((j & 1) == 0)  pos += term;
        else               neg += term;
    }
    return pos - neg;
}

/* ----------------- Möbius function via linear sieve ----------------------- */
static int  *mu        = NULL;   /* mu[0..mu_size]   */
static char *is_comp   = NULL;   /* composite flags  */
static int  *pr        = NULL;   /* list of primes   */
static size_t mu_size = 0;       /* current maximum index computed */

/* recompute mu[1..n] (linear sieve), discarding previous data */
static void recompute_mu_up_to(idx_t n)
{
    size_t limit = (size_t)n;
    int  *new_mu  = (int *)malloc((limit + 1) * sizeof(int));
    char *new_comp= (char*)calloc((limit + 1), sizeof(char));
    int  *new_pr  = (int *)malloc((limit + 1) * sizeof(int));
    if (!new_mu || !new_comp || !new_pr) { free(new_mu); free(new_comp); free(new_pr); exit(1); }

    new_mu[1] = 1;
    size_t pcnt = 0;
    for (size_t i = 2; i <= limit; ++i)
    {
        if (!new_comp[i])
        {
            new_pr[pcnt++] = (int)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt && (size_t)new_pr[j] * i <= limit; ++j)
        {
            size_t x = new_pr[j] * i;
            new_comp[x] = 1;
            if (i % new_pr[j] == 0)
            {
                new_mu[x] = 0;
                break;
            }
            else
                new_mu[x] = -new_mu[i];
        }
    }

    /* free old data */
    free(mu);      free(is_comp);  free(pr);
    mu   = new_mu;
    is_comp = new_comp;
    pr   = new_pr;
    mu_size = limit;
}

/* ensure mu[1..n] exists; return mu(n) */
static int mu_of(idx_t n)
{
    if (n == 0) return 0;
    if (n > mu_size) recompute_mu_up_to(n);
    return mu[(size_t)n];
}

/* ----------------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is defined only for n ≥ 1 */

    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            int mud = mu_of(d);
            if (mud != 0)
            {
                seq_t val = A001118(n / d);
                if (mud > 0) pos_sum += val;
                else         neg_sum += val;   /* mud == -1 */
            }
            /* complementary divisor n/d if distinct */
            idx_t e = n / d;
            if (e != d)
            {
                int mye = mu_of(e);
                if (mye != 0)
                {
                    seq_t val = A001118(n / e);  /* which is A001118(d) */
                    if (mye > 0) pos_sum += val;
                    else         neg_sum += val;
                }
            }
        }
    }
    return pos_sum - neg_sum;   /* result is non‑negative */
}
