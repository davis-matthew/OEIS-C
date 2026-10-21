/*
 * A239880  Number of strict partitions of n having an ordering in which no
 *          parts of equal parity are juxtaposed and the first and last terms
 *          have the same parity.
 * https://oeis.org/A239880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of strict partitions of n with an ordering * in
 *            which no parts of equal parity are adjacent * and the first and
 *            last parts have the same parity
 * Range:     u8 n <= 36, u16 n <= 91, u32 n <= 263, u64 n <= 845,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this is equivalent to counting strict partitions with odd number
 *       of parts and |#odd − #even| = 1.  */

#include <stdlib.h>
#include <stdbool.h>

/* global state */
static idx_t max_n_global   = 0;          /* highest n for which tables exist */
static idx_t stride_global  = 0;          /* = max_n_global + 1                */
static idx_t Emax_global    = 0;          /* maximum number of evens            */
static idx_t Omax_global    = 0;          /* maximum number of odds             */

static seq_t *dp_even      = NULL; /* dp_even[l * stride + s]  */
static seq_t *dp_odd       = NULL; /* dp_odd[k * stride + t]   */
static seq_t *ans_cache    = NULL; /* answer cache                */
static unsigned char *been    = NULL; /* 0/1 flag array               */

/* --------------------------------------------------------------------- */
/* recompute   rebuild all tables for N (inclusive)                       */
/* --------------------------------------------------------------------- */
static void recompute(idx_t N)
{
    /* free old tables */
    free(dp_even);
    free(dp_odd);
    free(ans_cache);
    free(been);

    max_n_global  = N;
    stride_global = N + 1U;

    /* maximal counts of distinct evens/odds that can sum to N */
    Emax_global = 0;
    while ((Emax_global + 1U) * (Emax_global + 2U) / 2U <= N) ++Emax_global;
    while (Emax_global * (Emax_global + 1U) / 2U > N) --Emax_global;

    Omax_global = 0;
    while ((Omax_global + 1U) * (Omax_global + 1U) <= N) ++Omax_global;
    /* rows needed for both tables */
    idx_t rows = (Emax_global > Omax_global ? Emax_global : Omax_global) + 1U;

    /* allocate dp tables */
    dp_even = (seq_t *)calloc(rows * stride_global, sizeof(seq_t));
    dp_odd  = (seq_t *)calloc(rows * stride_global, sizeof(seq_t));
    if (!dp_even || !dp_odd)
    {
        free(dp_even);
        free(dp_odd);
        dp_even = dp_odd = NULL;
        return;   /* allocation failure – caller will see unset tables */
    }

    dp_even[0] = 1;    /* 0 evens, sum 0 */
    dp_odd[0]  = 1;    /* 0 odds,  sum 0 */

    /* build dp_even: choose distinct even parts (2,4,6,...) */
    for (idx_t val = 2U; val <= N; val += 2U)
    {
        for (int l = (int)Emax_global; l >= 1; --l)
        {
            seq_t *row_l   = dp_even + (idx_t)l * stride_global;
            seq_t *row_lm1 = dp_even + (idx_t)(l-1) * stride_global;
            for (idx_t s = val; s <= N; ++s)
                row_l[s] += row_lm1[s - val];
        }
    }

    /* build dp_odd: choose distinct odd parts (1,3,5,...) */
    for (idx_t val = 1U; val <= N; val += 2U)
    {
        for (int k = (int)Omax_global; k >= 1; --k)
        {
            seq_t *row_k   = dp_odd + (idx_t)k * stride_global;
            seq_t *row_km1 = dp_odd + (idx_t)(k-1) * stride_global;
            for (idx_t s = val; s <= N; ++s)
                row_k[s] += row_km1[s - val];
        }
    }

    /* answer cache */
    ans_cache = (seq_t *)calloc(max_n_global + 1U, sizeof(seq_t));
    been      = (unsigned char *)calloc(max_n_global + 1U, sizeof(unsigned char));
}

/* --------------------------------------------------------------------- */
/* a   compute A239880(n)                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* rebuild tables when needed */
    if (n > max_n_global)
        recompute(n);

    /* guard allocation failure */
    if (!dp_even || !dp_odd || !ans_cache || !been)
        return 0;

    if (been[n])
        return ans_cache[n];

    seq_t total = 0U;
    idx_t stride = stride_global;
    idx_t Emax = Emax_global;
    idx_t Omax = Omax_global;

    /* case O = E + 1 */
    for (idx_t e = 0U; e <= Emax; ++e)
    {
        idx_t o = e + 1U;
        if (o > Omax)
            continue;
        idx_t base_e = e * stride;
        idx_t base_o = o * stride;
        for (idx_t s = 0U; s <= n; ++s)
        {
            idx_t t = n - s;
            total += dp_even[base_e + s] * dp_odd[base_o + t];
        }
    }

    /* case E = O + 1 */
    for (idx_t o = 0U; o <= Omax; ++o)
    {
        idx_t e = o + 1U;
        if (e > Emax)
            continue;
        idx_t base_e = e * stride;
        idx_t base_o = o * stride;
        for (idx_t s = 0U; s <= n; ++s)
        {
            idx_t t = n - s;
            total += dp_even[base_e + s] * dp_odd[base_o + t];
        }
    }

    ans_cache[n] = total;
    been[n]     = 1U;
    return total;
}
