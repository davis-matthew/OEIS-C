/*
 * A062122  Boustrophedon transform of 0, 1, 0, 2, 3, 5, 8, 13, 21, 34, 55,
 *          89, 144,... the Fibonacci numbers (F_0 = 0, F_1 = 1, A000045) with
 *          an erroneous term (F_2 = 0 instead of 1).
 * https://oeis.org/A062122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Boustrophedon transform of * b(0)=0, b(1)=1, b(2)=0,
 *            b(k)=Fib(k) for k≥3 (Fibonacci with F₂=0) * The transform rows
 *            are constructed alternately left‑to‑right and * right‑to‑left.
 *            This implementation memoises the previous row and the * sequence
 *            values so that any call a(n) runs in O(n) time...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of a(n) up to the largest n computed.  */
static seq_t *a_cache   = NULL;
static idx_t  a_cache_cap = 0;   /* capacity of a_cache   */
static idx_t  a_cache_len = 0;   /* number of values stored */

/* Cached Fibonacci numbers up to the largest index needed.  */
static seq_t *fib_cache  = NULL;
static idx_t  fib_cache_cap = 0; /* capacity of fib_cache   */
static idx_t  fib_cache_len = 0; /* number of values stored */

/* The last row of the Boustrophedon array.  */
static seq_t *row_prev   = NULL;
static idx_t  row_prev_len = 0;  /* number of elements in row_prev */

/* Ensure fib_cache can hold indices 0..n and contains correct Fibonacci
 * numbers (with F₂=1; the error is applied later).  */
static void ensure_fib(idx_t n)
{
    if (fib_cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fib_cache = tmp;
        fib_cache_cap = new_cap;
    }

    if (fib_cache_len < 2) {
        /* Initialise the first two Fibonacci numbers.  */
        if (fib_cache_len == 0) {
            fib_cache[0] = (seq_t)0;
        }
        if (fib_cache_len <= 1) {
            if (fib_cache_cap > 1)
                fib_cache[1] = (seq_t)1;
        }
        fib_cache_len = 2;
    }

    /* Compute missing Fibonacci numbers up to n.  */
    for (idx_t i = fib_cache_len; i <= n; ++i) {
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cache_len = n + 1;
}

/* Ensure a_cache has capacity for indices 0..n and that its length is
 * at least the number of stored values.  */
static void ensure_a_capacity(idx_t n)
{
    if (a_cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
}

/* Main sequence function.  Computes A062122.  */
seq_t a(idx_t n)
{
    /* Return cached value if already computed.  */
    if (n < a_cache_len) {
        return a_cache[n];
    }

    /* Grow caches as necessary.  */
    ensure_fib(n);
    ensure_a_capacity(n);

    /* Construct rows from the first missing index up to n.  */
    for (idx_t m = a_cache_len; m <= n; ++m) {
        idx_t len = m + 1;
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur) abort();

        /* Value b(m) used in the transform; override the erroneous 2nd
         * Fibonacci number.  */
        seq_t b_m = (m == 2 ? (seq_t)0 : fib_cache[m]);

        if (m == 0) {                 /* first row */
            cur[0] = b_m;
            a_cache[0] = cur[0];
        } else if (m & 1) {            /* odd row: left → right */
            cur[0] = b_m;
            for (idx_t k = 1; k <= m; ++k) {
                cur[k] = cur[k - 1] + row_prev[k - 1];
            }
            a_cache[m] = cur[m];
        } else {                      /* even row: right → left */
            cur[m] = b_m;
            for (idx_t k = m; k-- > 0; ) {
                cur[k] = cur[k + 1] + row_prev[k];
            }
            a_cache[m] = cur[0];
        }

        /* Update the cached row for the next iteration.  */
        free(row_prev);
        row_prev      = cur;
        row_prev_len  = len;
        a_cache_len   = m + 1;
    }

    return a_cache[n];
}
