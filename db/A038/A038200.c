/*
 * A038200  Row sums of triangle K(m, n), inverse to triangle T(m,n) in
 *          A020921.
 * https://oeis.org/A038200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0..n} (-1)^(n-k) * C(n,k) * τ(k)
 * where τ(k) is the divisor-count function (A000005) and
 * τ(0) := 0.
 * The sequence begins: 1, 0, -1, 3, -8, 21, ...
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached divisor counts up to the largest argument seen so far. */
static seq_t *tau_cache = NULL;
static idx_t tau_cached = 0;

/* ------------------------------------------------------------ */
/*  Compute τ(k) for all k ≤ n, extending the cache if necessary.  */
static void
compute_tau_up_to(idx_t n)
{
    if (n <= tau_cached)
        return;

    /* Allocate new cache with enough room. */
    seq_t *new_tau = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_tau)
        return;                   /* allocation failure – give up */

    /* Initialise new entries. */
    for (idx_t i = 0; i <= n; ++i)
        new_tau[i] = 0;

    /* Sieve‑style divisor counting. */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            new_tau[j] += 1;

    /* τ(0) = 0 by convention. */
    new_tau[0] = 0;

    /* Replace the old cache. */
    if (tau_cache)
        free(tau_cache);
    tau_cache   = new_tau;
    tau_cached  = n;
}

/* ------------------------------------------------------------ */
/*  Public function: compute a(n).  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of OEIS offset range */

    /* Ensure divisor counts are available. */
    compute_tau_up_to(n);

    /* Evaluate the binomial sum. */
    seq_t sum      = 0;
    seq_t binom    = 1;                 /* C(n,0) */
    int   sign     = (n & 1) ? -1 : 1;  /* (-1)^n */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom * tau_cache[k];
        if (sign < 0)
            term = -term;
        sum += term;

        /* Update binomial coefficient for the next k.  */
        if (k < n)
        {
            /* Compute C(n,k+1) = C(n,k)*(n-k)/(k+1) exactly. */
            seq_t tmp = (seq_t)(((__int128)binom * (n - k)) / (k + 1));
            binom = tmp;
        }
        sign = -sign;                  /* toggle sign for next term */
    }

    return sum;
}
