/*
 * A220116  Numbers k such that the number of 1's in binary expansion of k
 *          equals the number of terms in Zeckendorf representation of k.
 * https://oeis.org/A220116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 7634, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220116: Numbers k such that popcount(k) equals
   the number of terms in the Zeckendorf representation of k.
   Computed by incremental search with memoisation. */

#include <stdlib.h>
#include <stdint.h>

/* Fenwick helpers -------------------------------------------------------- */
static seq_t *fib      = NULL;
static size_t  fib_len = 0;
static size_t  fib_cap = 0;

/* Ensure Fibonacci array contains all Fibonacci numbers ≤ val. */
static void
ensure_fib_upto(seq_t val)
{
    /* initialise with F2=1, F3=2 if not yet done */
    if (fib_len == 0) {
        fib_cap = 4;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_len = 2;
    }

    while (fib[fib_len-1] < val) {
        /* grow array if necessary */
        if (fib_len == fib_cap) {
            fib_cap = fib_cap ? fib_cap * 2 : 4;
            fib = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
        }
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        ++fib_len;
    }
}

/* Count terms in Zeckendorf representation of x. 
   Uses the standard greedy algorithm. */
static idx_t
zeckendorf_count(seq_t x)
{
    idx_t count = 0;
    while (x) {
        ensure_fib_upto(x);
        /* find largest Fibonacci <= x */
        size_t i = fib_len - 1;
        while (fib[i] > x) --i;
        x -= fib[i];
        ++count;
    }
    return count;
}

/* Count number of 1-bits in x. Works for any unsigned integer size. */
static idx_t
popcount(seq_t x)
{
    idx_t cnt = 0;
    while (x) {
        x &= x - 1;
        ++cnt;
    }
    return cnt;
}

/* Sequence storage ------------------------------------------------------- */
static seq_t *seq     = NULL;
static idx_t  seq_len = 0;
static idx_t  seq_cap = 0;

/* Ensure seq has space for at least 'min_cap' elements. */
static void
ensure_seq_capacity(idx_t min_cap)
{
    if (seq_cap < (size_t)min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < (size_t)min_cap)
            new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Main function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for sanity */

    /* initialise first element */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* extend sequence until we have at least n entries */
    seq_t candidate = seq[seq_len - 1] + (seq_t)1;
    while (seq_len < n) {
        /* test candidate */
        if (popcount(candidate) == zeckendorf_count(candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len] = candidate;
            ++seq_len;
        }
        ++candidate;
    }

    return seq[n - 1];
}
