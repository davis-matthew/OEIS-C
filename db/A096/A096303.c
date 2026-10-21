/*
 * A096303  Number of iterations of n -> n + (number of 1's in binary
 *          representation of n) needed for the trajectory of n to join the
 *          trajectory of A010062.
 * https://oeis.org/A096303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 542, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each n, iterate n -> n + popcount(n) until the value
 * enters the trajectory of A010062 (starting at 1).  The return value is
 * the number of iterations required.  The routine maintains an on‑demand
 * cache of the A010062 sequence for efficient membership testing. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *S1_seq = NULL;         /* cached A010062 values */
static size_t S1_len = 0, S1_cap = 0;

/* Count the number of 1 bits in x.  Works for all supported seq_t types. */
static unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        if (x & (seq_t)1) ++c;
        x >>= 1;
    }
    return c;
}

/* Grow the cached A010062 sequence until it contains all values ≥ target. */
static void ensure_S1_up_to(seq_t target)
{
    if (!S1_seq) {
        S1_cap = 16;
        S1_seq = (seq_t *)malloc(S1_cap * sizeof(seq_t));
        if (!S1_seq) abort();           /* allocation failure */
        S1_seq[0] = (seq_t)1;
        S1_len = 1;
    }
    while (S1_seq[S1_len - 1] < target) {
        seq_t last = S1_seq[S1_len - 1];
        seq_t next = last + (seq_t)popcount_seq(last);
        if (S1_len == S1_cap) {
            S1_cap <<= 1;
            S1_seq = (seq_t *)realloc(S1_seq, S1_cap * sizeof(seq_t));
            if (!S1_seq) abort();
        }
        S1_seq[S1_len++] = next;
    }
}

/* Test whether x is in the A010062 sequence. */
static bool S1_contains(seq_t x)
{
    ensure_S1_up_to(x);
    size_t lo = 0, hi = S1_len;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (S1_seq[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < S1_len && S1_seq[lo] == x);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, defensive */
    seq_t cur = (seq_t)n;
    seq_t steps = (seq_t)0;
    while (!S1_contains(cur)) {
        cur = cur + (seq_t)popcount_seq(cur);
        ++steps;
    }
    return steps;
}
