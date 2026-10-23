/*
 * A162697  E.g.f. satisfies: A(x) = 1+x + x^2*exp(x*A(x)).
 * https://oeis.org/A162697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*G(n-2) where G satisfies G(0)=1, G(1)=1 and for
 *            n>=2 G(n) = G(n-1) + 2*(n-1)*G(n-2) + Σ_{k=3..n} k *
 *            G(k-3)*G(n-k) * ((n-1)! / ((k-3)!*(n-k)!)) Simplified: G(n) =
 *            G(n-1) + 2*(n-1)G(n-2) + Σ_{r=0..n-3} (r+3)*G(r)*G(n-r-3) *
 *            binom(n-1,r)*(n-r-2)*(n-r-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ---------- static cache for G values ---------- */
static seq_t *G = NULL;        /* G[n] values, index 0.. */
static idx_t G_len = 0;        /* number of filled elements (indices 0..G_len-1) */
static idx_t G_cap = 0;        /* allocated capacity */

/* ---------- helper to grow an array ---------- */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    *arr = seq_realloc(*arr, *cap, new_cap);
    *cap = new_cap;
}

/* ---------- build G up to index up_to ---------- */
static void ensure_G_up_to(idx_t up_to)
{
    if (up_to + 1 <= G_len) return; /* already computed */

    size_t new_cap = G_cap;
    if (new_cap == 0) new_cap = 8;
    while (new_cap <= up_to) new_cap *= 2;

    G = seq_realloc(G, G_cap, new_cap);
    G_cap = new_cap;

    if (G_len == 0) {                  /* first initialization */
        G[0] = (seq_t)1;
        if (new_cap > 1) G[1] = (seq_t)1;
        G_len = 2;
    } else if (G_len == 1) {            /* we only had G[0] before */
        G[1] = (seq_t)1;
        G_len = 2;
    }

    /* compute new entries */
    for (idx_t n = G_len; n <= up_to; ++n) {
        seq_t sum = (seq_t)0;

        /* G(n-1) term */
        sum += G[n-1];

        /* 2*(n-1)*G(n-2) term (only if n>=2, but loop guarantees it) */
        seq_t t2 = G[n-2];
        t2 *= (seq_t)(n-1);
        t2 *= (seq_t)2;
        sum += t2;

        /* convolution part for n >= 3 */
        if (n >= 3) {
            seq_t binom = (seq_t)1;        /* binom(n-1, r) for r=0 */
            for (idx_t r = 0; r <= n-3; ++r) {
                seq_t term = G[r];
                term *= G[n - r - 3];

                /* multiply by k = r+3 */
                term *= (seq_t)(r + 3);

                /* multiply by (n-r-2)*(n-r-1) */
                term *= (seq_t)(n - r - 2);
                term *= (seq_t)(n - r - 1);

                /* multiply by binom(n-1, r) */
                term *= binom;

                sum += term;

                /* update binom to next value for next r */
                if (r < n-3) {
                    /* binom_next = binom * (n-1 - r) / (r+1) */
                    seq_t mul = binom;
                    mul *= (seq_t)(n - 1 - r);
                    binom = mul / (seq_t)(r + 1);
                }
            }
        }

        G[n] = sum;
    }
    G_len = up_to + 1;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    /* we need G up to index n-2 */
    ensure_G_up_to(n - 2);

    seq_t res = (seq_t)n;
    res *= (seq_t)(n - 1);
    res *= G[n - 2];
    return res;
}
