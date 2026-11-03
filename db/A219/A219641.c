/*
 * A219641  a(n) = n minus (number of 1's in Zeckendorf expansion of n).
 * https://oeis.org/A219641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A007895(n) (number of 1's in the Zeckendorf expansion
 *            of n)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8762 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Helper: pre‑compute Fibonacci numbers F1 = 1, F2 = 2 that fit in idx_t. */
static void init_fibs(idx_t *fibs, size_t *len)
{
    if (*len != 0)            /* already initialized */
        return;

    fibs[0] = 1;              /* F1 */
    fibs[1] = 2;              /* F2 */
    *len = 2;

    while (1) {
        idx_t next = fibs[*len - 1] + fibs[*len - 2];
        if (next < fibs[*len - 1])   /* unsigned overflow detection */
            break;
        fibs[(*len)++] = next;
    }
}

/* Helper: binary search for the largest Fibonacci number <= n */
static size_t largest_fib_idx(const idx_t *fibs, size_t len, idx_t n)
{
    size_t lo = 0;
    size_t hi = len - 1;

    while (lo < hi) {
        size_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (fibs[mid] <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;   /* fibs[lo] <= n < fibs[lo+1] (if any) */
}

/* ------------------------------------------------------------------ */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 0 */
        return (seq_t)0;

    /* Static cache of Fibonacci numbers.  The array is large enough for
       the maximum value that fits in idx_t (uint64_t). */
    static idx_t fibs[94];
    static size_t fib_len = 0;
    if (fib_len == 0)
        init_fibs(fibs, &fib_len);   /* one‑time initialization */

    idx_t remaining = n;
    seq_t ones = 0;                  /* count of 1's in Zeckendorf representation */

    while (remaining > 0) {
        /* Find the largest Fibonacci <= remaining */
        size_t idx = largest_fib_idx(fibs, fib_len, remaining);
        remaining -= fibs[idx];
        ones += (seq_t)1;          /* increase count of 1's */
    }

    return (seq_t)n - ones;
}
