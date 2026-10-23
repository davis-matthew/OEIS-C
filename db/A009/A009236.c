/*
 * A009236  E.g.f. exp(sinh(x)^2) (even powers only).
 * https://oeis.org/A009236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009236
 *  The sequence is the coefficient of x^(2n)/ (2n)! in
 *  exp(sinh(x)^2).  Let c_m be the ordinary e.g.f coefficient
 *  of exp(sinh(x)^2):
 *
 *   c_0 = 1
 *   For n ≥ 0
 *     c_{n+1} = Σ_{k=0..n} binom(n,k) · c_k · 2^{n-k}
 *                (sum over k such that (n-k) is odd)
 *
 *  Then a(n) = c_{2n}  (for n ≥ 0).
 *
 *  The function below precomputes the array c up to the
 *  required index on the first call, caching results for
 *  subsequent calls.  Only integer arithmetic is used; all
 *  operations are performed with type seq_t, which may be
 *  any unsigned integer type up to 128 bits or an
 *  arbitrary‑precision integer class.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Global cache of e.g.f coefficients c_m */
static seq_t *c = NULL;          /* c[0] .. c[maxComputed] */
static idx_t   c_size = 0;       /* number of elements allocated */

/* Power of two pre‑computed for quick access */
static seq_t *pow2 = NULL;       /* pow2[i] = 2^i */
static idx_t   pow2_size = 0;    /* number of elements allocated */

/* Forward declarations */
static void ensure_powers(idx_t target);
static void ensure_c(idx_t target);

/* Ensure pow2 array is large enough for indices up to 'target' */
static void ensure_powers(idx_t target)
{
    if (pow2_size > target)
        return;

    if (!pow2)
    {
        pow2 = seq_alloc(target + 1);
        pow2[0] = 1;
        pow2_size = 1;
    }
    else
    {
        pow2 = seq_realloc(pow2, pow2_size, target + 1);
    }

    for (idx_t i = pow2_size; i <= target; ++i)
    {
        pow2[i] = pow2[i - 1] * 2;
    }
    pow2_size = target + 1;
}

/* Ensure c array is large enough for indices up to 'target' */
static void ensure_c(idx_t target)
{
    if (c_size > target)
        return;

    if (!c)
    {
        c = seq_alloc(target + 1);
        c[0] = 1;
        c_size = 1;
    }
    else
    {
        c = seq_realloc(c, c_size, target + 1);
    }

    /* Compute new c values sequentially */
    for (idx_t n = c_size - 1; n < target; ++n)
    {
        /* binomial coefficient initialized to C(n,0) = 1 */
        seq_t binom = 1;
        seq_t sum  = 0;

        for (idx_t k = 0; k <= n; ++k)
        {
            /* Add term only if (n-k) is odd */
            if ((n - k) & 1U)
            {
                /* term = binom * c[k] * 2^(n-k) */
                seq_t term = binom * c[k];
                term = term * pow2[n - k];
                sum += term;
            }

            /* Update binomial coefficient to C(n, k+1) */
            if (k < n)
            {
                binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            }
        }

        c[n + 1] = sum;
    }
    c_size = target + 1;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* The required index in c is 2*n */
    idx_t target = 2 * n;

    /* Expand power‑of‑two table first */
    ensure_powers(target);

    /* Expand c array as needed */
    ensure_c(target);

    return c[target];
}
