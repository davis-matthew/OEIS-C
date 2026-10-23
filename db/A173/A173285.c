/*
 * A173285  A(x) satisfies: Fibonacci(x)/x = A(x)/A(x^2).
 * https://oeis.org/A173285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} F(n–2k+1) * a(k)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   with a(0) = 1, F(0)=0, F(1)=1  (F = Fibonacci numbers) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc.h" /* seq_alloc / seq_realloc provide ctor/destructor */
#include "../utils/seq_realloc.h"

static seq_t *a_vals  = NULL;   /* memoised a(n) values */
static size_t a_cap   = 0;      /* allocated capacity for a_vals */
static idx_t  a_max   = (idx_t)-1;   /* highest n whose a(n) is known */

static seq_t *fib_vals  = NULL;   /* Fibonacci numbers */
static size_t fib_cap   = 0;      /* allocated capacity for fib_vals */
static idx_t  fib_max   = (idx_t)-1;   /* highest Fibonacci index computed */

/* Ensure a_vals can hold indices up to n (inclusive) */
static void
ensure_a_capacity(idx_t n)
{
    if (n < a_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = seq_realloc(a_vals, a_cap, new_cap);
    if (!tmp) abort();
    a_vals = tmp;
    a_cap  = new_cap;
}

/* Ensure fib_vals can hold F(n) up to index n (inclusive) */
static void
ensure_fib_capacity(idx_t n)
{
    if (n < fib_cap) return;
    size_t new_cap = (size_t)n + 1;

    if (fib_cap == 0) {                     /* first allocation */
        fib_vals = seq_alloc(new_cap);
        if (!fib_vals) abort();
        fib_vals[0] = (seq_t)0;
        fib_vals[1] = (seq_t)1;
        fib_max     = 1;
        fib_cap     = new_cap;
    } else {
        seq_t *tmp = seq_realloc(fib_vals, fib_cap, new_cap);
        if (!tmp) abort();
        fib_vals = tmp;
        fib_cap  = new_cap;
    }

    /* Compute any missing Fibonacci numbers */
    for (idx_t i = fib_max + 1; i <= n; ++i) {
        fib_vals[i] = fib_vals[i - 1] + fib_vals[i - 2];
    }
    fib_max = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Initialise memoisation structure if needed */
    if (a_vals == NULL) {
        a_vals = seq_alloc(1);
        if (!a_vals) abort();
        a_vals[0] = (seq_t)1;   /* a(0) = 1 */
        a_cap      = 1;
        a_max      = 0;
    }

    /* Ensure Fibonacci numbers up to n+1 are available */
    ensure_fib_capacity(n + 1);

    /* If already computed, return it */
    if (n <= a_max) return a_vals[n];

    /* Ensure a_vals has space */
    ensure_a_capacity(n);

    /* Compute all missing a(i) from a_max+1 up to n */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t lim = i >> 1;              /* floor(i/2) */
        for (idx_t k = 0; k <= lim; ++k) {
            seq_t fib_val = fib_vals[i - (k << 1) + 1];
            sum = sum + fib_val * a_vals[k];
        }
        a_vals[i] = sum;
    }
    a_max = n;
    return a_vals[n];
}
