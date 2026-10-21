/*
 * A078801  Expansion of Gaussian product of arithmetic mean and Lehmer mean
 *          evaluated at 1 + 4*x.
 * https://oeis.org/A078801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 23, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078801: a(n) = (-1)^n * Σ_{k>0} 2^(n-2k) * a(k) * binomial(n-2, n-2k), n>1
   a(0)=1, a(1)=2, a(2)=2 */

#include <stdlib.h>

/* Compute binomial(N,K) using multiplicative formula; N,K are non‑negative.
   The result always fits in seq_t, and the intermediate division is exact. */
static seq_t binom_small(int N, int K)
{
    if (K < 0 || K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (int i = 1; i <= K; ++i)
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* (1) static cache for sequence values   */
    static seq_t *cache = NULL;   /* cache[0..max_n]           */
    static idx_t  cap   = 0;      /* allocated size            */
    static idx_t  max_n = 0;      /* highest index computed    */

    /* (2) static cache for powers of two  */
    static seq_t *pow2 = NULL;    /* pow2[i] = 2^i              */
    static idx_t  pow_cap = 0;    /* allocated size            */
    static idx_t  pow_max  = 0;    /* highest power computed    */

    /* initialise on first call */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 2;
        max_n   = 2;

        pow_cap = 8;
        pow2    = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        if (!pow2) return 0;
        pow2[0] = 1;
        pow_max = 0;
    }

    /* grow cache for sequence if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* grow cache for powers of two if necessary */
    if (n >= pow_cap) {
        idx_t new_cap = pow_cap;
        while (new_cap <= n) new_cap <<= 1;
        pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        pow_cap = new_cap;
    }

    /* compute missing powers of two */
    for (idx_t i = pow_max + 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;
    pow_max = n;

    /* compute terms up to n if not already done */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(0), a(1), a(2) are already known */
        if (i <= 2) {
            cache[i] = (i == 0) ? 1 : ((i == 1) ? 2 : 2);
            continue;
        }

        idx_t max_k = i / 2;   /* floor(i/2) */
        int N = (int)(i - 2);   /* N = i-2, used for binomial */
        int r = N;              /* current exponent in binomial: r = N - 2*(k-1) */
        seq_t binom_val = 1;    /* binom(N, r) for r = N => 1 */
        seq_t sum = 0;

        for (idx_t k = 1; k <= max_k; ++k) {
            seq_t pow_term = pow2[r];
            seq_t term = cache[k] * pow_term * binom_val;
            sum += term;

            /* update binom_val for next k (if any) */
            if (k < max_k) {
                /* binom(N, r-2) = binom_val * r * (r-1) / ((N - r + 2) * (N - r + 1)) */
                seq_t numerator   = (seq_t)r * (seq_t)(r - 1);
                seq_t denom_part1 = (seq_t)(N - r + 2);
                seq_t denom_part2 = (seq_t)(N - r + 1);
                seq_t denom       = denom_part1 * denom_part2;
                binom_val = (binom_val * numerator) / denom;
                r -= 2;
            }
        }

        cache[i] = ((i & 1) ? -sum : sum);   /* (-1)^i * sum */
    }

    max_n = (n > max_n) ? n : max_n;
    return cache[n];
}
