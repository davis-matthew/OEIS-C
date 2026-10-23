/*
 * A134774  G.f.: A(x) = Product_{n>=1} G(x^n,n)^n where G(x,n) = 1 +
 *          x*G(x,n)^n.
 * https://oeis.org/A134774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, my_gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A134774 – a(n) = (1 / n) * Σ_{k=1..n} A105862(k) * a(n−k)
    where A105862(k) = Σ_{d|k} binomial(k,d)·k/gcd(k,d)
    a(0) = 1
    First terms: 1, 1, 3, 6, 15, 26, 66, 110, 253, 460, 966, … (OEIS) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */
#include "../utils/my_gcd_idx_t.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Global memoised storage for a(n) and A105862(n). */
static seq_t *A   = NULL;      /* a[n]        */
static seq_t *W   = NULL;      /* A105862[n]  */
static idx_t   max_n = 0;      /* highest computed index */

/* ------------------------------------------------------------------ */
/* Ensure that values up to n are computed and stored. */
static void
ensure_upto(idx_t n)
{
    /* Allocate or grow the storage arrays as needed. */
    if (A == NULL) {
        size_t sz = (size_t)(n + 1);
        A = (seq_t *)malloc(sz * sizeof(seq_t));
        W = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!A || !W) exit(EXIT_FAILURE);
        A[0] = (seq_t)1;            /* a(0) */
        max_n = 0;
    } else if (n <= max_n) {
        return; /* already prepared */
    } else {
        size_t sz = (size_t)(n + 1);
        A = (seq_t *)realloc(A, sz * sizeof(seq_t));
        W = (seq_t *)realloc(W, sz * sizeof(seq_t));
        if (!A || !W) exit(EXIT_FAILURE);
    }

    /* Compute all new entries from (max_n+1) up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* ---- compute W[k] = A105862(k) --------------------------- */
        seq_t w_sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d) continue;
            idx_t g = my_gcd_idx_t(k, d);
            seq_t term = binomial(k, d) * ((seq_t)(k / g));
            w_sum += term;
            idx_t d2 = k / d;
            if (d2 != d) {
                g = my_gcd_idx_t(k, d2);
                term = binomial(k, d2) * ((seq_t)(k / g));
                w_sum += term;
            }
        }
        W[k] = w_sum;

        /* ---- compute a[k] using the recurrence ----------------- */
        seq_t acc = 0;
        for (idx_t i = 1; i <= k; ++i) {
            acc += W[i] * A[k - i];
        }
        A[k] = acc / (seq_t)k;  /* exact division */
    }

    max_n = n;
}

/* ------------------------------------------------------------------ */
/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return A[n];
}
