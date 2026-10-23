/*
 * A047682  a(n) = 4^(2*n)*(4^(2*n)-1)*Bernoulli(2*n)/(2*n).
 * https://oeis.org/A047682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4^(2*n)*(4^(2*n)-1)*Bernoulli(2*n)/(2*n)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 8, i128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_gcd.h"
#include "../utils/binomial.h"

typedef struct {
    seq_t num;   /* numerator (may be negative) */
    seq_t den;   /* positive denominator */
} rat_t;

static rat_t *bernoulli = NULL;   /* array of B_0 .. B_m (m reached so far) */
static size_t bern_max  = 0;      /* largest index already computed   */
static size_t bern_cap  = 0;      /* allocated capacity of bernoulli */

/* Reduce a rational number to lowest terms. */
static void rat_reduce(rat_t *r)
{
    if (r->den < 0) {
        r->den = -r->den;
        r->num = -r->num;
    }
    seq_t g = seq_gcd((r->num >= 0 ? r->num : -r->num), r->den);
    if (g > 1) {
        r->num /= g;
        r->den /= g;
    }
}

/* Ensure B_0 .. B_n are all computed. */
static void ensure_bernoulli(size_t n)
{
    if (n <= bern_max) return;

    /* Grow the array to hold indices up to n. */
    if (bern_cap <= n) {
        size_t new_cap = bern_cap ? bern_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        rat_t *tmp = (rat_t*)realloc(bernoulli, new_cap * sizeof(rat_t));
        if (!tmp) abort();
        bernoulli = tmp;
        bern_cap = new_cap;
    }

    /* Initialize missing entries. */
    for (size_t i = bern_max; i <= n; ++i) {
        if (i == 0) {                   /* B0 = 1 */
            bernoulli[0].num = 1;
            bernoulli[0].den = 1;
        } else if (i == 1) {            /* B1 = -1/2 */
            bernoulli[1].num = -1;
            bernoulli[1].den = 2;
        } else if (i % 2 == 1) {        /* odd > 1 : 0 */
            bernoulli[i].num = 0;
            bernoulli[i].den = 1;
        } else {
            /* Compute B_i using the standard recurrence:
               (i+1)*B_i = - ∑_{k=0}^{i-1} binom(i+1, k) * B_k
               B_i = -S / (i+1)   where S = ∑_{k=0}^{i-1} ...
            */
            rat_t sum = {0, 1};     /* rational sum */
            for (size_t k = 0; k < i; ++k) {
                if (bernoulli[k].num == 0) continue;
                seq_t C = binomial(i + 1, k);           /* binom(i+1,k) */
                seq_t t_num = C * bernoulli[k].num;
                seq_t t_den = bernoulli[k].den;
                /* Add term to sum: sum_num/sum_den + t_num/t_den */
                seq_t new_num = sum.num * t_den + t_num * sum.den;
                seq_t new_den = sum.den * t_den;
                rat_t temp = {new_num, new_den};
                rat_reduce(&temp);
                sum = temp;
            }
            /* B_i = -sum / (i+1) */
            rat_t bi;
            bi.num = -sum.num;
            bi.den = sum.den * (seq_t)(i + 1);
            rat_reduce(&bi);
            bernoulli[i] = bi;
        }
    }
    bern_max = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Out of range */

    size_t idx = 2 * (size_t)n;          /* we need B_{2n} */
    ensure_bernoulli(idx);

    /* 4^(2n) */
    seq_t power = big_pow((seq_t)4, (seq_t)(2 * n));
    seq_t power_minus1 = power - 1;

    /* Denominator part: B_den * (2n) */
    seq_t denom = bernoulli[idx].den * (seq_t)(idx);

    /* Multiply numerators, cancelling with denom when possible. */
    seq_t prod = 1;
    /* Phase 1: multiply by 4^(2n) */
    seq_t g = seq_gcd(power >= 0 ? power : -power, denom);
    if (g > 1) { power /= g; denom /= g; }
    prod *= power;

    /* Phase 2: multiply by (4^(2n)-1) */
    g = seq_gcd(power_minus1 >= 0 ? power_minus1 : -power_minus1, denom);
    if (g > 1) { power_minus1 /= g; denom /= g; }
    prod *= power_minus1;

    /* Phase 3: multiply by Bernoulli numerator */
    g = seq_gcd(bernoulli[idx].num >= 0 ? bernoulli[idx].num : -bernoulli[idx].num, denom);
    seq_t bnum = bernoulli[idx].num;   /* keep sign */
    if (g > 1) { bnum /= g; denom /= g; }
    prod *= bnum;

    /* Final division by remaining denom */
    if (denom != (seq_t)1) prod /= denom;

    return prod;
}
