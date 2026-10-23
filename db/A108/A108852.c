/*
 * A108852  Number of Fibonacci numbers <= n.
 * https://oeis.org/A108852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of Fibonacci numbers ≤ n (counting 0 and the duplicate 1)
 * Range:     u8 n <= 676, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental generation of Fibonacci numbers up to n,
   storing them in a static array and using a binary search to count. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fib_arr = NULL;   /* array of Fibonacci numbers */
static idx_t fib_cap = 0;       /* allocated capacity */
static idx_t fib_len = 0;       /* number of Fibonacci numbers stored */

/* Ensure that fib_arr has at least min_cap entries. */
static void ensure_fib_capacity(idx_t min_cap)
{
    if (fib_cap >= min_cap) return;
    while (fib_cap < min_cap) fib_cap = fib_cap ? fib_cap * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(fib_arr, fib_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    fib_arr = tmp;
}

/* Return a(n) = number of Fibonacci numbers ≤ n. */
seq_t a(idx_t n)
{
    /* Lazy initialization of Fibonacci array */
    if (!fib_arr) {
        fib_cap = 4;
        fib_arr = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_arr) exit(EXIT_FAILURE);
        fib_arr[0] = (seq_t)0;   /* F0 */
        fib_arr[1] = (seq_t)1;   /* F1 */
        fib_arr[2] = (seq_t)1;   /* F2 (duplicate 1) */
        fib_len  = 3;
    }

    seq_t target = (seq_t)n;

    /* Generate Fibonacci numbers until the first one > n (or overflow). */
    while (fib_len >= 2) {
        seq_t next = fib_arr[fib_len - 1] + fib_arr[fib_len - 2];

        /* Detect overflow for fixed-size unsigned types: next < previous. */
        if (next < fib_arr[fib_len - 1] || next > target) break;

        ensure_fib_capacity(fib_len + 1);
        fib_arr[fib_len++] = next;
    }

    /* Binary search to count how many stored Fibonacci numbers are ≤ n. */
    idx_t lo = 0, hi = fib_len;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (fib_arr[mid] <= target)
            lo = mid + 1;
        else
            hi = mid;
    }

    return (seq_t)lo;
}
