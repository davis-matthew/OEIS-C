/*
 * A023487  a(n) = b(n) + d(n), where b(n) = (n-th Lucas number > 3) and d(n)
 *          = (n-th non-Fibonacci number).
 * https://oeis.org/A023487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000032(n+3) + A001690(n+1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached Lucas numbers, indexed by k */
static seq_t *lucas_cache = NULL;
static size_t lucas_cap = 0;
static idx_t lucas_len = 0;

/* Ensure that lucas_cache contains values up to index k */
static void ensure_lucas(idx_t k)
{
    if (lucas_len > k)
        return;

    /* grow capacity */
    size_t new_cap = lucas_cap ? lucas_cap : 16;
    if (new_cap <= (size_t)k) {
        while (new_cap <= (size_t)k)
            new_cap <<= 1;
    }

    seq_t *tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    lucas_cache = tmp;
    lucas_cap = new_cap;

    /* initialise first terms if needed */
    if (lucas_len == 0) {
        lucas_cache[0] = (seq_t)2;
        lucas_len = 1;
    }
    if (lucas_len == 1) {
        lucas_cache[1] = (seq_t)1;
        lucas_len = 2;
    }

    /* compute missing Lucas numbers */
    for (idx_t i = lucas_len; i <= k; ++i) {
        lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
    }
    lucas_len = k + 1;
}

/* Return the n-th non–Fibonacci number (0‑based). */
static seq_t nth_non_fib(idx_t n)
{
    seq_t f_prev = (seq_t)1;
    seq_t f_next = (seq_t)2;
    idx_t count = 0;

    while (1) {
        seq_t low  = f_prev + (seq_t)1;
        seq_t high = f_next - (seq_t)1;

        if (low <= high) {
            /* how many integers are in this gap */
            idx_t missing = (idx_t)(high - low + (seq_t)1);
            if (n < count + missing)
                return low + (seq_t)(n - count);
            count += missing;
        }

        /* move to the next Fibonacci pair */
        seq_t f_curr = f_prev + f_next;
        f_prev = f_next;
        f_next = f_curr;
    }
}

/* Entry point for the sequence */
seq_t a(idx_t n)
{
    /* b(n) = Lucas(n+3) */
    ensure_lucas(n + 3);
    seq_t b_val = lucas_cache[n + 3];

    /* d(n) = n-th non‑Fibonacci number (0‑based) */
    seq_t d_val = nth_non_fib(n);

    return b_val + d_val;
}
