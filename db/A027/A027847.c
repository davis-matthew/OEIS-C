/*
 * A027847  a(n) = Sum_{d|n} sigma(n/d)*d^3.
 * https://oeis.org/A027847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} sigma(n/d)*d^3
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a table of sigma(m) for all m up to the
   current maximum n requested.  That table is rebuilt whenever a
   larger n is needed; for n ≤ 10 000 this rebuild costs only about
   O(n log n) operations and is therefore fast.  Divisors are found
   by a simple loop up to sqrt(n).  All arithmetic is performed in
   the build‑dependent type seq_t, so the routine works for unsigned
   8‑, 16‑, 32‑, 64‑bit or arbitrary‑precision integer types.  */

#include <stddef.h>   /* size_t */

static seq_t *sigma_tab = NULL;   /* sigma values, indexed from 0..N */
static idx_t  sigma_max   = 0;    /* current maximum n for which sigma_tab is valid */

/* --- ensure that sigma_tab contains sigma values for all numbers <= n --- */
static void
ensure_sigma(idx_t n)
{
    if (n <= sigma_max)
        return;

    /* allocate new array of size n + 1 (index 0 unused) */
    seq_t *new_tab = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_tab)
        return;   /* in production code we would handle the error */

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i)
        new_tab[i] = (seq_t)0;

    /* populate sigma values using the divisor‑summing sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            new_tab[j] += (seq_t)i;
        }
    }

    /* free old table if it existed */
    if (sigma_tab)
        free(sigma_tab);

    sigma_tab = new_tab;
    sigma_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* make sure sigma values up to n are available */
    ensure_sigma(n);

    seq_t total = (seq_t)0;
    idx_t root = 0;

    /* compute integer square root of n */
    while ((root + 1) * (root + 1) <= n)
        ++root;

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d != 0)
            continue;

        idx_t q = n / d;        /* complement divisor */

        /* term for divisor d */
        seq_t d3        = (seq_t)d * (seq_t)d * (seq_t)d;
        seq_t sigma_q   = sigma_tab[q];
        total += sigma_q * d3;

        /* if d != q, also add the term for divisor q */
        if (d != q) {
            seq_t q3        = (seq_t)q * (seq_t)q * (seq_t)q;
            seq_t sigma_d   = sigma_tab[d];
            total += sigma_d * q3;
        }
    }

    return total;
}
