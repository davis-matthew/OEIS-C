/*
 * A105774  A "fractal" transform of the Fibonacci numbers: a(1)=1; then if
 *          F(n) < k <= F(n+1), a(k) = F(n+1) - a(k - F(n)) where F(n) =
 *          A000045(n).
 * https://oeis.org/A105774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 5364, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS style): a(1)=1; for k with F(n)<k<=F(n+1), a(k)=F(n+1)-a(k-F(n))
   (F is the Fibonacci sequence with F(1)=1, F(2)=1).  The implementation
   uses memoisation and a dynamic Fibonacci table, working for any
   integer type seq_t and index type idx_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Fibonacci table (1‑based, fib[1]=1, fib[2]=1). */
static seq_t *fib   = NULL;  /* array of seq_t */
static size_t fib_cap = 0;   /* allocated capacity */
static size_t fib_len = 0;   /* number of elements stored (including dummy 0) */

/* Memoisation table for a(n).  memo[0] is unused. */
static seq_t *memo     = NULL;  /* array of seq_t */
static size_t memo_cap = 0;     /* allocated capacity */

/* Ensure the Fibonacci table contains at least indices 0..m and
   that fib[m-1] > target_n (so that fib[m-1] is the first Fib > target_n). */
static void ensure_fib(idx_t target_n)
{
    if (!fib) {
        fib_cap = 16;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib) exit(EXIT_FAILURE);
        fib[0] = 0;            /* dummy */
        fib[1] = 1;
        fib[2] = 1;
        fib_len = 3;           /* elements stored indices 0..2 */
    }

    /* Grow until we have an index fib_len-1 that is > target_n and
       at least one more element available for the upper bound. */
    while (fib_len <= target_n + 1 || fib[fib_len-1] <= (seq_t)target_n) {
        if (fib_len >= fib_cap) {
            fib_cap <<= 1;
            seq_t *p = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!p) { free(fib); exit(EXIT_FAILURE); }
            fib = p;
        }
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        fib_len++;
    }
}

/* Ensure memoisation table can hold indices up to n. */
static void ensure_memo(idx_t n)
{
    if (!memo || memo_cap <= n) {
        size_t new_cap = memo_cap ? memo_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!p) { free(memo); exit(EXIT_FAILURE); }
        if (!memo) {
            for (size_t i = 0; i < new_cap; ++i) p[i] = 0;
        } else {
            for (size_t i = memo_cap; i < new_cap; ++i) p[i] = 0;
        }
        memo = p;
        memo_cap = new_cap;
    }
}

/* Recursive helper with memoisation.  All indices are 1‑based. */
static seq_t a_rec(idx_t n)
{
    if (n == 1) return 1;

    ensure_memo(n);
    if (memo[n] != 0) return memo[n];

    ensure_fib(n);

    /* Binary search for m: fib[m] < n <= fib[m+1] */
    idx_t low  = 1;
    idx_t high = fib_len - 1;        /* fib[high] >= n because fib_len grew above */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (fib[mid] < (seq_t)n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    idx_t m = low;
    idx_t offset = n - (idx_t)fib[m];            /* n > fib[m] guaranteed */
    seq_t val = fib[m + 1] - a_rec(offset);
    memo[n] = val;
    return val;
}

/* Public accessor required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range; not used by the host */
    return a_rec(n);
}
