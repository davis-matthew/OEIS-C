/*
 * A063022  Reversion of y - y^2 - y^3 - y^5.
 * https://oeis.org/A063022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Reversion of y - y^2 - y^3 - y^5
    a(n) satisfies the functional equation g(t) - g(t)^2 - g(t)^3 - g(t)^5 = t
    where g(t) = Σ_{k≥1} a(k) t^k.  The coefficients can be computed
    recursively:

        a(k) = [t^k](g^2) + [t^k](g^3) + [t^k](g^5) + δ_{k,1}
              = b2(k) + b3(k) + b5(k) + δ_{k,1}

    with
        b2(k) = Σ_{i=1}^{k-1} a(i)·a(k-i)
        b3(k) = Σ_{i=1}^{k-1} a(i)·b2(k-i)
        b5(k) = Σ_{i=1}^{k-1} b2(i)·b3(k-i)

    These recurrences use only previously computed terms, so the sequence
    may be generated in O(n²) time, which is fast enough for n≈10 000. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache for coefficients a(k), b2(k), b3(k), b5(k) and current size. */
    static seq_t *ac    = NULL,
                  *b2c   = NULL,
                  *b3c   = NULL,
                  *b5c   = NULL;
    static idx_t  acap = 0;           /* cached length (including index 0) */

    /* Ensure cache at least up to index n. */
    if (acap <= n) {
        idx_t new_cap = (n + 1) * 2;          /* grow factor to amortise reallocs */
        if (new_cap < n + 1) new_cap = n + 1;
        if (acap == 0) {
            ac    = (seq_t*)malloc(new_cap * sizeof(seq_t));
            b2c   = (seq_t*)malloc(new_cap * sizeof(seq_t));
            b3c   = (seq_t*)malloc(new_cap * sizeof(seq_t));
            b5c   = (seq_t*)malloc(new_cap * sizeof(seq_t));
            memset(ac,   0, new_cap * sizeof(seq_t));
            memset(b2c,  0, new_cap * sizeof(seq_t));
            memset(b3c,  0, new_cap * sizeof(seq_t));
            memset(b5c,  0, new_cap * sizeof(seq_t));
        } else {
            ac    = (seq_t*)realloc(ac,    new_cap * sizeof(seq_t));
            b2c   = (seq_t*)realloc(b2c,   new_cap * sizeof(seq_t));
            b3c   = (seq_t*)realloc(b3c,   new_cap * sizeof(seq_t));
            b5c   = (seq_t*)realloc(b5c,   new_cap * sizeof(seq_t));
            memset(ac + acap, 0, (new_cap - acap) * sizeof(seq_t));
            memset(b2c + acap, 0, (new_cap - acap) * sizeof(seq_t));
            memset(b3c + acap, 0, (new_cap - acap) * sizeof(seq_t));
            memset(b5c + acap, 0, (new_cap - acap) * sizeof(seq_t));
        }
        acap = new_cap;
    }

    /* a(0) = 0 is already in cache by initialization. */
    if (n == 0) return (seq_t)0;

    /* If the value is already computed, return it. */
    if (ac[n] != (seq_t)0) return ac[n];

    /* Compute all coefficients up to n incrementally. */
    for (idx_t k = 1; k <= n; ++k) {
        if (ac[k] != (seq_t)0) continue;         /* already computed in previous call */

        /* Compute b2(k) = Σ a[i]*a[k-i] */
        seq_t s2 = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            s2 += ac[i] * ac[k - i];
        b2c[k] = s2;

        /* Compute b3(k) = Σ a[i]*b2(k-i) */
        seq_t s3 = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            s3 += ac[i] * b2c[k - i];
        b3c[k] = s3;

        /* Compute b5(k) = Σ b2(i)*b3(k-i) */
        seq_t s5 = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            s5 += b2c[i] * b3c[k - i];
        b5c[k] = s5;

        /* a(k) = b2(k) + b3(k) + b5(k) + δ_{k,1} */
        seq_t ak = s2 + s3 + s5;
        if (k == 1) ak += (seq_t)1;
        ac[k] = ak;
    }

    return ac[n];
}
