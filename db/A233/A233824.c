/*
 * A233824  A recurrent sequence in Panaitopol's formula for pi(x), where
 *          pi(x) is the number of primes <= x.
 * https://oeis.org/A233824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A233824
    Formula: a(n) = n*n! - Σ_{k=1}^{n-1} k!*a(n-k)
    (a(n) = A003319(n+1) for n>0)                               */

#include <stddef.h>

static seq_t   *fact   = NULL;   /* factorial[i] = i!                 */
static seq_t   *a_mem  = NULL;   /* a_mem[i]   = a(i)                 */
static size_t   cap    = 0;       /* current allocation capacity       */
static idx_t    max_n  = 0;       /* largest n for which a_mem is filled */

/* Grow the factorial and a_mem arrays to at least capacity (n+1). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap)
        return;

    size_t new_cap = needed * 2;
    seq_t *new_fact;
    seq_t *new_a;

#ifdef __cplusplus
    /* Allocate new C++ arrays (constructs objects).  */
    new_fact = new seq_t[new_cap];
    new_a    = new seq_t[new_cap];
    if (cap == 0) {                     /* first allocation: initialise base case */
        new_fact[0] = (seq_t)1;
        new_a[0]    = (seq_t)0;
    }
    /* Copy existing data.                     */
    for (size_t i = 0; i < cap; ++i) {
        new_fact[i] = fact[i];
        new_a[i]    = a_mem[i];
    }
    delete[] fact;
    delete[] a_mem;
    fact  = new_fact;
    a_mem = new_a;
#else
    /* Allocate with C realloc (contents are uninitialised).      */
    new_fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
    new_a    = (seq_t*)realloc(a_mem, new_cap * sizeof(seq_t));
    if (cap == 0) {                     /* first allocation: initialise base case */
        new_fact[0] = (seq_t)1;
        new_a[0]    = (seq_t)0;
    }
    fact  = new_fact;
    a_mem = new_a;
#endif
    cap = new_cap;
}

/* Compute a(n) for arbitrary order of calls.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* a(0) = 0 */

    if ((idx_t)max_n < n) {
        ensure_capacity(n);

        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* factorial[i] = factorial[i-1] * i */
            fact[i] = fact[i - 1] * (seq_t)i;

            /* sum_{k=1}^{i-1} k!*a(i-k) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k)
                sum += fact[k] * a_mem[i - k];

            /* a(i) = i*i! - sum */
            a_mem[i] = (seq_t)i * fact[i] - sum;
        }
        max_n = n;
    }
    return a_mem[n];
}
