/*
 * A076995  a(1) = 4, a(n+1) is the largest composite number < 2a(n).
 * https://oeis.org/A076995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A076995
 *
 * a(1) = 4
 * a(n+1) = the largest composite number < 2*a(n)
 *
 * Recurrence simplification:
 *   a(n+1) = 2*a(n) - 2  if 2*a(n)-1 is prime
 *   a(n+1) = 2*a(n) - 1  otherwise
 *   (since 2*a(n)-2 is always composite when 2*a(n)-1 is prime)
 *
 * The implementation memoizes all computed values so that calls
 * with arbitrary n are fast.  The only expensive operation is
 * the primality check of a potentially huge odd number; this
 * uses the provided is_prime_big() routine.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *memo = NULL;     /* memoized a(1), a(2), ... */
static size_t memo_len = 0;    /* number of values stored */
static size_t memo_cap = 0;    /* allocated capacity */

/* Allocate or grow memo array to hold at least 'cap' elements. */
static void ensure_memo_capacity(size_t cap)
{
    if (cap <= memo_cap) return;
    size_t new_cap = memo_cap == 0 ? 1 : memo_cap * 2;
    while (new_cap < cap) new_cap *= 2;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) {
        /* Out of memory: abort as no error handling is required. */
        abort();
    }
    memo = new_mem;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1: terms start at n=1 */
        return (seq_t)0;
    }

    /* Convert idx_t to seq_t for operations */
    seq_t idx_seq = (seq_t)n;

    /* Expand memo if necessary */
    if ((size_t)n > memo_len) {
        ensure_memo_capacity((size_t)n);
        /* Compute missing terms */
        for (size_t i = memo_len; i < (size_t)n; ++i) {
            if (i == 0) {
                memo[i] = (seq_t)4;                     /* a(1) */
                continue;
            }
            seq_t prev = memo[i - 1];
            seq_t candidate = (seq_t)2 * prev - (seq_t)1; /* 2*a(n-1) - 1 */

            if (is_prime_big(candidate)) {
                memo[i] = candidate - (seq_t)1;         /* 2*a-2 */
            } else {
                memo[i] = candidate;                    /* 2*a-1 */
            }
        }
        memo_len = (size_t)n;
    }

    return memo[(size_t)n - 1];
}
