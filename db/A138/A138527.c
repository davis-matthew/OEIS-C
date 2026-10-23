/*
 * A138527  Expansion of phi(-q) / phi(-q^5) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A138527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in φ(-q)/φ(-q^5) * Recurrence: * a(0)
 *            = 1 * For n ≥ 1: * n*a(n) = - Σ_{k=1}^{n} delta[k] * a(n-k) *
 *            where * delta[k] = Σ_{d|k} d * e(d) * e(d) = { 0 if d mod 10 ∈
 *            {0,5} * 1 if d mod 10 ∈ {2,4,6,8} * 2 if d mod 10 ∈ {1,3,7,9} }
 * Range:     i8 n <= 15, i16 n <= 60, i32 n <= 330, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t cached_n = 0;

/* Ensure that a_cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n) return;

    idx_t size = n + 1;
    seq_t *new_a = (seq_t *)malloc(size * sizeof(seq_t));
    if (!new_a) return;          /* allocation failure -- silently fail */

    /* temporary array for delta */
    seq_t *delta = (seq_t *)calloc(size, sizeof(seq_t));
    if (!delta) { free(new_a); return; }

    /* compute delta[k] for 1 ≤ k ≤ n */
    for (idx_t d = 1; d <= n; ++d)
    {
        int mod = d % 10;
        int e_val = 0;
        if (mod == 0 || mod == 5)        e_val = 0;
        else if (mod == 1 || mod == 3 || mod == 7 || mod == 9) e_val = 2;
        else if (mod == 2 || mod == 4 || mod == 6 || mod == 8) e_val = 1;

        if (e_val == 0) continue;
        seq_t prod = (seq_t)d * e_val;
        for (idx_t m = d; m <= n; m += d)
            delta[m] += prod;
    }

    /* compute a[0 … n] */
    new_a[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
            sum += delta[k] * new_a[i - k];
        new_a[i] = -(sum / (seq_t)i);
    }

    free(delta);
    free(a_cache);
    a_cache = new_a;
    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return a_cache[n];
}
