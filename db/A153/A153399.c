/*
 * A153399  G.f.: A(x) = F(x*G(x)^3) where F(x) = G(x/F(x)) = 1 + x*F(x)^3 is
 *          the g.f. of A001764 and G(x) = F(x*G(x)) = 1 + x*G(x)^4 is the
 *          g.f. of A002293.
 * https://oeis.org/A153399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} [ C(3k+1,k)/(3k+1) * C(4n–k,n–k) *
 *            3k/(4n–k) ] for n>0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* helper: grow a dynamic array to capacity >= need+1 */
static void grow_to(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap > need) return;
    size_t new_cap = (*cap > 0) ? *cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* storage for pre‑computed values */
static seq_t *f_arr = NULL;   /* f[k] = C(3k+1,k)/(3k+1) */
static size_t f_cap = 0, f_len = 0;

static seq_t *a_arr = NULL;   /* a[n] */
static size_t a_cap = 0, a_len = 0;

/* compute f[0..max_n] */
static void ensure_f(size_t max_n)
{
    grow_to(&f_arr, &f_cap, max_n);
    for (size_t i = f_len; i <= max_n; ++i) {
        if (i == 0) {          /* f(0) = 1 */
            f_arr[0] = (seq_t)1;
            continue;
        }
        idx_t k = (idx_t)i;
        /* recurrence: f(k) = f(k-1) * (3k-2)(3k-1)(3k) / (k*(2k)*(2k+1)) */
        seq_t num = (seq_t)(3*k-2) * (seq_t)(3*k-1);
        num *= (seq_t)(3*k);
        seq_t den = (seq_t)k * (seq_t)(2*k);
        den *= (seq_t)(2*k+1);
        f_arr[i] = f_arr[i-1] * num / den;
    }
    f_len = max_n + 1;
}

/* compute a[0..max_n] */
static void ensure_a(size_t max_n)
{
    if (max_n == 0) {
        grow_to(&a_arr, &a_cap, 0);
        a_arr[0] = 1;
        a_len = 1;
        return;
    }

    ensure_f(max_n);
    grow_to(&a_arr, &a_cap, max_n);
    for (size_t i = a_len; i <= max_n; ++i) {
        if (i == 0) {         /* never hit here because max_n>0, but keep safety */
            a_arr[0] = 1;
            continue;
        }
        idx_t n = (idx_t)i;
        seq_t sum = 0;
        /* B = C(4n,n) */
        seq_t B = binom(4*n, n);
        for (idx_t k = 1; k <= n; ++k) {
            /* update B to C(4n-k, n-k) */
            seq_t num = B * (n - (k-1));
            seq_t den = (seq_t)(4*n - (k-1));
            B = num / den;

            seq_t term = f_arr[k];
            term *= B;
            term *= 3*k;
            term /= (4*n - k);
            sum += term;
        }
        a_arr[i] = sum;
    }
    a_len = max_n + 1;
}

/* required sequence function */
seq_t a(idx_t n)
{
    size_t mn = (size_t)n;
    ensure_a(mn);
    return a_arr[mn];
}
