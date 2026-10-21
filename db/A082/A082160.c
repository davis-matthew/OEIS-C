/*
 * A082160  Deterministic completely defined acyclic automata with 3 inputs
 *          and n+1 transient labeled states including a unique state having
 *          all transitions to the absorbing state.
 * https://oeis.org/A082160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = b_3(n) where * b_3(0) = 1, * b_3(n) = Σ_{i=0..n-1}
 *            binomial(n,i) * (-1)^(n-i-1) * * (((i+2)^3 - 1)^(n-i)) * b_3(i)
 *            for n > 0 * * This implementation memoises the values and
 *            evaluates the recurrence * directly. It works for both
 *            fixed‑width (e.g. uint64_t) and * arbitrary‑precision (seq_t)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute binomial coefficient C(n,k) as seq_t using multiplicative formula.
   The division is exact at each step. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* Fast exponentiation: pow_seq(base, exp) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result = result * b;
        b = b * b;
        e >>= 1;
    }
    return result;
}

/* Memoised array of previously computed b_3 values */
static seq_t *cache = NULL;   /* array of size cache_size */
static size_t cache_size = 0; /* number of allocated elements */

/* Ensure the cache can hold values up to index 'n' (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;
    size_t new_size = n + 1;
    /* Grow by 1.5x to reduce reallocs if possible */
    if (new_size < cache_size * 3 / 2) new_size = cache_size * 3 / 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);                 /* memory error */
    cache = new_cache;
    /* Initialise new entries to zero */
    for (size_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
    cache_size = new_size;
}

/* Recursive helper that fills cache[n] using the recurrence */
static seq_t compute_b(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != (seq_t)0) return cache[n];
    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;

    for (idx_t i = 0; i < n; ++i) {
        if (cache[i] == (seq_t)0) cache[i] = compute_b(i);

        seq_t c = binom_seq(n, i);
        int sign_negative = ((n - i - 1) & 1);      /* 1 if negative sign */

        seq_t base = ((seq_t)(i + 2) * (seq_t)(i + 2) * (seq_t)(i + 2)) - (seq_t)1;
        seq_t p = pow_seq(base, n - i);
        seq_t term = c * p * cache[i];

        if (sign_negative) neg_sum += term;
        else                pos_sum += term;
    }

    seq_t result = pos_sum - neg_sum;   /* pos_sum should be ≥ neg_sum for all n */
    cache[n] = result;
    return result;
}

/* Public sequence function; handles arbitrary order of calls */
seq_t a(idx_t n)
{
    return compute_b(n);
}
