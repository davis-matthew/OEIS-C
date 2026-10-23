/*
 * A174477  G.f.: exp( Sum_{n>=1} A174478(n)*x^n/n ) where A174478(n) =
 *          Sum_{d|n} d*2^(n/d)*tau(d).
 * https://oeis.org/A174477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1 a(n) = (1/n) * sum_{k=1}^{n} [ A174478(k) *
 *            a(n-k) ] where A174478(k) = sum_{d|k} d * 2^(k/d) * τ(d) (τ =
 *            divisor count)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes A174478 for all 1..n and then uses
   the recurrence above.  All indices are handled exactly as in the OEIS
   offset (a(0)=first term).  The function is fully memoised; subsequent
   calls with larger n extend the pre‑computation, while calls with a
   smaller n simply return the previously stored value. */

#include <stdlib.h>

static seq_t *a_arr   = NULL;   /* a(n) values                       */
static seq_t *s_arr   = NULL;   /* A174478(k) values                 */
static seq_t *pow2    = NULL;   /* 2^i          for i=0..n          */
static idx_t *tau_arr = NULL;   /* τ(i) for i=0..n                    */
static idx_t  sz      = 0;      /* largest n computed so far          */

/* Compute all auxiliary tables up to at least n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= sz && a_arr != NULL)
        return;  /* already ready */

    /* Allocate or grow storage */
    a_arr   = (seq_t *)realloc(a_arr,   (size_t)(n+1) * sizeof(seq_t));
    s_arr   = (seq_t *)realloc(s_arr,   (size_t)(n+1) * sizeof(seq_t));
    pow2    = (seq_t *)realloc(pow2,    (size_t)(n+1) * sizeof(seq_t));
    tau_arr = (idx_t *)realloc(tau_arr, (size_t)(n+1) * sizeof(idx_t));

    /* ---- compute τ(i) for i=0..n ---- */
    for (idx_t i = 0; i <= n; ++i)
        tau_arr[i] = 0;
    for (idx_t d = 1; d <= n; ++d)
        for (idx_t m = d; m <= n; m += d)
            ++tau_arr[m];

    /* ---- pre‑compute powers of two ---- */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i-1] * (seq_t)2;

    /* ---- compute A174478(k) for k=1..n ---- */
    for (idx_t k = 1; k <= n; ++k)
        s_arr[k] = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t td = tau_arr[d];
        for (idx_t m = d; m <= n; m += d) {
            idx_t exp = m / d;                     /* k/d   */
            seq_t term = pow2[exp];
            term = term * (seq_t)d;
            term = term * (seq_t)td;
            s_arr[m] += term;
        }
    }

    /* ---- compute a(n) via recurrence ---- */
    a_arr[0] = (seq_t)1;  /* a(0)=1 by definition */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
            sum += s_arr[k] * a_arr[m-k];
        a_arr[m] = sum / (seq_t)m;
    }

    sz = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1; /* a(0)=1 by definition */
    compute_up_to(n);
    return a_arr[n];
}
