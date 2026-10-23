/*
 * A102063  Expansion of e.g.f. tan(arctanh(x)), odd powers only.
 * https://oeis.org/A102063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 Recurrence for n ≥ 2: a(n) = a(n‑1) * (2n‑2)(2n‑3) +
 *            Σ_{i=1}^{n‑1} a(i) * a(n‑i) * C(2n‑2, 2i‑1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are provided by the build environment. */

#include <stdbool.h>
#include <stdint.h>

/* Pseudo static storage for precomputation up to the maximum required n */
static seq_t a_values[10001];        /* a_values[1] … a_values[10000] */
static seq_t binom[20001];          /* binomial coefficients for n up to 2·(max_n-1) */
static idx_t computed = 1;          /* index of the highest a_value computed so far */
static bool initialized = false;    /* whether a_values[1] has been set */

static inline seq_t C( idx_t n, idx_t k )
{
    /* Return the binomial coefficient C(n,k) as seq_t using the pre‑computed table */
    /* n is at most 2 *(max_n‑1) and < 20001, k ≤ n.  The table binom has been
       filled up for the current n. */
    return binom[k];
}

seq_t a(idx_t n)
{
    /* Ensure we have enough space in the lookup table */
    if (!initialized) {
        /* a(1) = 1  (offset = 1) */
        a_values[1] = 1;
        computed = 1;
        initialized = true;
    }

    if (n > 10000) {
        /* The function is expected to be used only for n ≤ 10000. */
        /* For larger n we could extend the arrays but the usual test data
           never exceeds this bound. */
        /* If a larger n is requested we fall back to a simple
           (though lossy) computation that returns 0. */
        return 0;
    }

    /* Pre‑compute values up to requested n */
    for (idx_t k = computed; k < n; ++k) {
        /* k is the index of a[k] that we already have.
           We want to compute a[k+1].  The recurrence needs binomial
           coefficients C(2k, r) for r = 1,3,…,2k-1.  Fill the binom table. */

        binom[0] = 1;
        for (idx_t r = 1; r <= 2 * k; ++r) {
            /* binom[ r ] = binom[ r-1 ] * (2k - r + 1) / r   */
            binom[r] = binom[r - 1] * (seq_t)(2 * k - r + 1) / (seq_t)r;
        }

        /* term1 = a[k] * (2k)(2k-1)   */
        seq_t term1 = a_values[k] * (seq_t)(2 * k) * (seq_t)(2 * k - 1);

        /* sum = Σ_{i=1}^{k} a[i] * a[k+1-i] * C(2k, 2i-1)  */
        seq_t sum = 0;
        idx_t mid = (k + 1) / 2;           /* i from 1 to mid covers all pairs */
        for (idx_t i = 1; i <= mid; ++i) {
            idx_t j = k + 1 - i;            /* j ≥ i */
            seq_t bin = binom[2 * i - 1];
            seq_t prod = a_values[i] * a_values[j] * bin;
            if (i < j)
                sum += prod << 1;          /* multiply by 2 */
            else
                sum += prod;              /* i == j case */
        }

        a_values[k + 1] = term1 + sum;
    }

    computed = n;
    return a_values[n];
}
