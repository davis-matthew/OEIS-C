/*
 * A023024  Number of partitions of n into 4 unordered relatively prime parts.
 * https://oeis.org/A023024
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * b(n/d) * where b(m) = partitions of m into
 *            parts ≤4 minus partitions of m into parts ≤3 * (i.e. number of
 *            unordered partitions of m into exactly 4 parts). * b(m) is
 *            computed by DP with parts 1..4 and 1..3 respectively. * The
 *            Möbius function μ is computed by a linear sieve
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t max_precalc = 0;
static seq_t *b_cache = NULL;
static signed char *mu_cache = NULL;

/* Ensure all needed data up to at least n is pre‑computed. */
static void ensure_precalc(idx_t n)
{
    if (n <= max_precalc) return;

    idx_t new_size = max_precalc ? max_precalc * 2 : 4;
    while (new_size < n) new_size *= 2;

    seq_t *new_b = (seq_t *)realloc(b_cache, (new_size + 1) * sizeof(seq_t));
    if (!new_b) abort();
    b_cache = new_b;
    signed char *new_mu = (signed char *)realloc(mu_cache, (new_size + 1) * sizeof(signed char));
    if (!new_mu) abort();
    mu_cache = new_mu;

    /* Möbius function up to new_size via linear sieve. */
    int *lp = (int *)calloc(new_size + 1, sizeof(int));
    int *pr = (int *)malloc((new_size + 1) * sizeof(int));
    size_t pcnt = 0;
    if (new_size >= 1) mu_cache[1] = 1;
    for (idx_t i = 2; i <= new_size; ++i) {
        if (lp[i] == 0) {
            lp[i] = (int)i;
            pr[pcnt++] = (int)i;
            mu_cache[i] = -1;
        }
        for (size_t j = 0; j < pcnt && (size_t)pr[j] <= (size_t)lp[i]
                && i * (idx_t)pr[j] <= new_size; ++j) {
            idx_t k = i * (idx_t)pr[j];
            lp[k] = pr[j];
            if ((size_t)pr[j] == (size_t)lp[i]) mu_cache[k] = 0;
            else mu_cache[k] = -mu_cache[i];
        }
    }
    free(lp);
    free(pr);

    /* DP for partitions into parts ≤4 (dp4) and parts ≤3 (dp3). */
    seq_t *dp4 = (seq_t *)calloc(new_size + 1, sizeof(seq_t));
    seq_t *dp3 = (seq_t *)calloc(new_size + 1, sizeof(seq_t));
    dp4[0] = 1;
    dp3[0] = 1;
    for (idx_t s = 1; s <= 4; ++s) {
        for (idx_t i = s; i <= new_size; ++i) dp4[i] += dp4[i - s];
    }
    for (idx_t s = 1; s <= 3; ++s) {
        for (idx_t i = s; i <= new_size; ++i) dp3[i] += dp3[i - s];
    }
    for (idx_t i = 0; i <= new_size; ++i) b_cache[i] = dp4[i] - dp3[i];
    free(dp4);
    free(dp3);

    max_precalc = new_size;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    ensure_precalc(n);

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            signed char mu_d = mu_cache[d];
            if (mu_d == 1)      sum_pos += b_cache[e];
            else if (mu_d == -1) sum_neg += b_cache[e];

            if (d != e) {
                signed char mu_e = mu_cache[e];
                if (mu_e == 1)      sum_pos += b_cache[d];
                else if (mu_e == -1) sum_neg += b_cache[d];
            }
        }
    }

    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
