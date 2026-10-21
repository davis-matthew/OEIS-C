/*
 * A064608  Partial sums of A034444: sum of number of unitary divisors from 1
 *          to n.
 * https://oeis.org/A064608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} 2^{ω(k)} (ω(k) = number of distinct prime
 *            factors of k) Implementation uses a simple sieve to compute ω(k)
 *            up to the maximum argument seen so far and stores the cumulative
 *            sums. The algorithm is O(n log log n) and keeps all intermediate
 *            values within the native integer range
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state to allow memoisation across calls */
static idx_t _max_n = 0;          /* highest n that has been computed so far     */
static uint8_t * _omega = NULL;   /* ω(k) for k <= _max_n (distinct prime count)*/
static seq_t  * _cum  = NULL;     /* cumulative sums a(k) for k <= _max_n       */

/* extend the precomputed tables to accommodate all indices up to n */
static void _ensure_up_to(idx_t n)
{
    if (n <= _max_n) return;                 /* nothing to do */

    /* allocate new arrays */
    uint8_t * omega_new = (uint8_t*)malloc((n + 1) * sizeof(uint8_t));
    seq_t  * cum_new   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!omega_new || !cum_new) return; /* let the caller handle the failure */

    /* initialise 0 */
    memset(omega_new, 0, (n + 1) * sizeof(uint8_t));
    memset(cum_new,   0, (n + 1) * sizeof(seq_t));

    /* compute ω(k) via a simple prime‑sieve style loop */
    for (idx_t i = 2; i <= n; ++i) {
        if (omega_new[i] == 0) {            /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                omega_new[j]++;
        }
    }

    /* cumulative sums of 2^{ω(k)} */
    cum_new[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = ((seq_t)1 << omega_new[i]); /* 2^{ω(i)} */
        cum_new[i] = cum_new[i-1] + val;
    }

    /* discard old tables */
    free(_omega);
    free(_cum);

    /* install the new tables */
    _omega = omega_new;
    _cum   = cum_new;
    _max_n = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behaviour for indices below the offset */

    _ensure_up_to(n);
    return _cum[n];
}
