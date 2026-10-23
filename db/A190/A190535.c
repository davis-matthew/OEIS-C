/*
 * A190535  Number of (n+2) X (n+2) symmetric binary matrices without the
 *          pattern 0 1 1 diagonally.
 * https://oeis.org/A190535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56 * ∏_{k=7}^{n+5} (F_k - 1) (F_k = k‑th Fibonacci
 *            number)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Thread‑safe static cache of computed terms */
static idx_t a_cached   = 0;   /* highest n computed so far */
static seq_t *a_cache   = NULL;/* array a[0..n] , index 0 unused */

/* Cache for Fibonacci numbers */
/* fib_arr[i] = F_i   (F_0 = 0, F_1 = 1) */
static idx_t fib_len   = 0;   /* current number of Fibonacci numbers stored */
static seq_t *fib_arr  = NULL;/* array of Fibonacci numbers */

/* Ensure fib_arr contains Fibonacci numbers up to index need (inclusive) */
static void ensure_fib(idx_t need)
{
    if (fib_len > need) return;

    idx_t new_len = need + 1; /* indices 0..need */
    fib_arr = (seq_t *)realloc(fib_arr, new_len * sizeof(seq_t));
    if (fib_arr == NULL) exit(1); /* allocation failed */

    /* Initialise first two Fibonacci numbers if needed */
    if (fib_len == 0) {
        fib_arr[0] = 0;
        fib_arr[1] = 1;
        fib_len = 2;
    }

    /* Compute new Fibonacci numbers */
    for (idx_t i = fib_len; i < new_len; ++i)
        fib_arr[i] = fib_arr[i-1] + fib_arr[i-2];

    fib_len = new_len;
}

/* Ensure a_cache contains a[1..n] */
static void ensure_a(idx_t n)
{
    if (n <= a_cached) return;

    idx_t new_len = n + 1; /* indices 0..n */
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (a_cache == NULL) exit(1);

    /* Initialise first term if necessary */
    if (a_cached == 0) {
        a_cache[1] = 56;       /* a(1) = 56 */
        a_cached = 1;
    }

    /* Compute new terms in order */
    for (idx_t i = a_cached + 1; i <= n; ++i) {
        idx_t fib_idx = i + 5;                /* we need F_{i+5}   */
        ensure_fib(fib_idx);                  /* guarantee Fibonacci number available */
        seq_t mult = fib_arr[fib_idx] - 1;     /* F_{i+5} - 1      */
        a_cache[i] = a_cache[i-1] * mult;     /* recurrence step  */
    }

    a_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    ensure_a(n);
    return a_cache[n];
}
