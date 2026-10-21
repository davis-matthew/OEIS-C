/*
 * A051336  Number of increasing arithmetic progressions in {1,2,3,...,n},
 *          including trivial arithmetic progressions of lengths 1 and 2.
 * https://oeis.org/A051336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1; for n ≥ 2 a(n) = a(n-1) + 1 + Σ_{i=1}^{n-1} τ(i),
 *            where τ(i) is the number of positive divisors of i
 * Range:     u8 n <= 14, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state: divisor counts and sequence values */
static seq_t *tau_arr = NULL;      /* divisor counts, indices 0..max_n */
static idx_t  tau_len = 0;         /* tau_arr size (max_n+1) */

static seq_t *a_arr   = NULL;      /* sequence values, indices 0..max_n */
static idx_t  a_max   = 0;         /* largest n computed for a() */
static idx_t  a_len   = 0;         /* current allocated size of a_arr */
static seq_t  cum_tau = 0;         /* Σ_{k=1}^{current} τ(k)  */

/* ------------------------------------------------------------------
   Compute divisor counts τ(1)..τ(n) and store in tau_arr.
   The array is reallocated if necessary; all entries are recomputed.
   ------------------------------------------------------------------ */
static void extend_tau(idx_t n)
{
    if (tau_len > n + 1)               /* already computed up to n */
        return;

    idx_t new_len = n + 1;             /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(tau_arr, new_len * sizeof(seq_t));
    if (!tmp) abort();
    tau_arr = tmp;

    /* Zero the entire array before filling divisor counts */
    memset(tau_arr, 0, new_len * sizeof(seq_t));

    /* classical divisor sieve */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++tau_arr[j];

    tau_len = new_len;
}

/* ------------------------------------------------------------------
   Extend a_arr up to n using the recurrence and the already
   available τ values.
   ------------------------------------------------------------------ */
static void extend_a(idx_t n)
{
    if (n <= a_max)
        return;                     /* nothing to do */

    /* Ensure divisor counts are ready */
    extend_tau(n);

    /* Allocate or grow a_arr */
    idx_t new_len = n + 1;           /* indices 0..n */
    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!a_arr) abort();
        a_len = new_len;
    } else if (a_len < new_len) {
        seq_t *tmp = (seq_t *)realloc(a_arr, new_len * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        a_len = new_len;
    }

    /* Compute missing values of a() */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        if (i == 1) {
            a_arr[1] = (seq_t)1;          /* base case a(1)=1 */
            cum_tau = tau_arr[1];         /* Σ_{k=1}^{1} τ(k) */
        } else {
            /* a(i) = a(i-1) + 1 + Σ_{k=1}^{i-1} τ(k) */
            a_arr[i] = a_arr[i - 1] + (seq_t)1 + cum_tau;
            cum_tau += tau_arr[i];        /* update cumulative τ */
        }
    }
    a_max = n;
}

/* ------------------------------------------------------------------
   Public interface: returns a(n) for n ≥ 1.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined below offset */
        return 0;

    extend_a(n);
    return a_arr[n];
}
