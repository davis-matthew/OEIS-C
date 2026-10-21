/*
 * A225751  Number of different figures obtained by a putting two Young
 *          diagrams of partitions lambda and mu, such that |lambda| + |mu| =
 *          n on top of each other.
 * https://oeis.org/A225751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=ceil(n/2)..n} p(i) ( p = partition numbers )
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 107, u64 n <= 372,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Storage for partition numbers p[0..] and their prefix sums pref[0..] */
static seq_t *p      = NULL;   /* p[i] = partition number of i */
static seq_t *pref   = NULL;   /* pref[i] = sum_{k=0..i} p[k]           */
static idx_t  cap    = 0;      /* allocated size of p and pref         */
static idx_t  size   = 0;      /* number of elements already computed  */

/* Ensure that p[0..n] and pref[0..n] are computed and stored. */
static void
ensure_up_to(idx_t n)
{
    if (n < size)
        return; /* already computed */

    /* Allocate or grow the buffers.  Use power‑of‑two growth. */
    idx_t new_cap = cap;
    if (new_cap == 0)
        new_cap = 1;
    while (new_cap <= n)
        new_cap <<= 1;

    if (new_cap != cap) {
        p    = (seq_t *)realloc(p,    new_cap * sizeof(seq_t));
        pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
        cap  = new_cap;
    }

    /* Compute missing entries one by one, using the recurrence based on
       generalized pentagonal numbers.  This works for all integer types
       that support +, -, and comparisons. */
    if (size == 0) {
        /* p[0] = 1 is the base case.  pref[0] follows immediately. */
        p[0]    = (seq_t)1;
        pref[0] = p[0];
        size    = 1;
    }

    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            /* already handled above */
            continue;
        }

        seq_t val = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;

            int sign = (k & 1) ? 1 : -1;   /* (-1)^{k-1} */

            if (sign == 1)
                val += p[i - g1];
            else
                val -= p[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= i) {
                if (sign == 1)
                    val += p[i - g2];
                else
                    val -= p[i - g2];
            }
        }

        p[i] = val;
        pref[i] = pref[i - 1] + p[i];
    }

    size = n + 1;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    ensure_up_to(n);

    /* a(0) = 1 by definition (the empty diagram on top of the empty
       diagram).  For n > 0 we use the prefix sum trick. */
    if (n == 0)
        return (seq_t)1;

    idx_t half = (n + (idx_t)1) >> 1;   /* ceil(n/2) for unsigned index */
    seq_t res = pref[n];
    if (half > 0)
        res -= pref[half - 1];
    return res;
}
