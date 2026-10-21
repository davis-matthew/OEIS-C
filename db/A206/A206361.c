/*
 * A206361  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than two equal edges, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A206361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A206361
 *
 * Empirical recurrence for n > 8:
 *   a(n) = 16*a(n-1) - 42*a(n-2) + 234*a(n-3) - 784*a(n-4)
 *          + 714*a(n-5) - 609*a(n-6) + 1008*a(n-7)
 *
 * Initial values:
 *   a(1)=15, a(2)=151, a(3)=1990, a(4)=27548,
 *   a(5)=383021, a(6)=5327971, a(7)=74147694, a(8)=1031960617
 *
 * The function memoizes results so that any order of calls is fast.
 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *memo = NULL;      /* array of values, memo[0] unused */
static idx_t memo_size = 0;     /* largest index currently stored */

/* Ensure that memo has space for indices 1..n and that all values up to n are computed. */
static void ensure_cache(idx_t n)
{
    /* Lazy initialization: create space for the first 8 terms. */
    if (!memo) {
        size_t init_cap = 9;                 /* indices 0..8 */
        memo = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!memo) return;                    /* OOM: caller may not handle, but sequence build expected to succeed */

        /* Initial values for indices 1..8. */
        /* memo[0] is left unused. */
        memo[1] = (seq_t)15;
        memo[2] = (seq_t)151;
        memo[3] = (seq_t)1990;
        memo[4] = (seq_t)27548;
        memo[5] = (seq_t)383021;
        memo[6] = (seq_t)5327971;
        memo[7] = (seq_t)74147694;
        memo[8] = (seq_t)1031960617;
        memo_size = 8;
    }

    if (n <= memo_size)
        return;

    /* Extend the array to accommodate indices up to n. */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap <= memo_size) return; /* should not happen */

    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) return;                 /* OOM: caller may not handle */
    memo = new_mem;

    /* Compute missing terms using the recurrence. */
    for (idx_t k = memo_size + 1; k <= n; ++k) {
        if (k <= 8) {
            /* already initialized during first allocation */
            continue;
        }
        seq_t t = (seq_t)16  * memo[k - 1]
                - (seq_t)42  * memo[k - 2]
                + (seq_t)234 * memo[k - 3]
                - (seq_t)784 * memo[k - 4]
                + (seq_t)714 * memo[k - 5]
                - (seq_t)609 * memo[k - 6]
                + (seq_t)1008 * memo[k - 7];
        memo[k] = t;
    }
    memo_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1; safe value */

    ensure_cache(n);
    return memo[n];
}
