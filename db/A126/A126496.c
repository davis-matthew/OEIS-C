/*
 * A126496  Number of base 28 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126496: Number of base 28 n‑digit numbers with adjacent digits
 * differing by three or less.  The routine uses dynamic programming with
 * memoisation; it returns a(0)=1, a(1)=28, a(2)=184, ...
 *
 * It works for any seq_t defined by the build (built‑in integer types or an
 * arbitrary‑precision integer class).  For a C++ build, placement new is
 * used to construct the objects returned by malloc/realloc; for a C build
 * seq_t is a plain integer type and no construction is required.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
#include <new>
#endif

/* Helper to construct a seq_t object at *p when compiling as C++. */
static void init_seq_t(seq_t *p) {
#ifdef __cplusplus
    /* Placement new constructs a default‑initialised object. */
    new (p) seq_t();
#else
    (void)p;  /* nothing to do for plain integer types */
#endif
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Static memoisation table: memo[i] = a(i) for i < memo_size. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;          /* number of entries currently stored */

    /* Current‑length and next‑length counts for each of the 28 digits. */
    /* The 28‑element static arrays are zero‑initialised when the program
       starts, and are subsequently written before use. */
    static seq_t cur[28];
    static seq_t next[28];

    /* a(0) is defined to be 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Lazy initialisation of the memo table and the first‑length DP. */
    if (memo == NULL) {
        /* Allocate space for a(0) and a(1). */
        memo = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!memo) abort();

        /* Construct each element (no‑op for built‑in types). */
        for (idx_t i = 0; i < 2; ++i) {
            init_seq_t(&memo[i]);
        }

        memo[0] = (seq_t)1;  /* a(0) */

        /* For length 1, every digit is allowed: all 28 possibilities. */
        for (idx_t d = 0; d < 28; ++d) {
            cur[d] = (seq_t)1;
        }
        seq_t sum1 = 0;
        for (idx_t d = 0; d < 28; ++d) {
            sum1 += cur[d];
        }
        memo[1] = sum1;      /* a(1) = 28 */

        memo_size = 2;
    }

    /* Extend the memo table up to the requested index n. */
    while (memo_size <= n) {
        /* Compute counts for numbers of length memo_size (next length). */
        for (idx_t d = 0; d < 28; ++d) {
            next[d] = (seq_t)0;
        }

        for (idx_t d = 0; d < 28; ++d) {
            seq_t val = cur[d];
            if (!val) continue;

            idx_t lo = (d >= 3) ? d - 3 : 0;
            idx_t hi = (d <= 24) ? d + 3 : 27;

            for (idx_t d2 = lo; d2 <= hi; ++d2) {
                next[d2] += val;
            }
        }

        /* Sum the counts for all ending digits to get a(memo_size). */
        seq_t sumk = 0;
        for (idx_t d = 0; d < 28; ++d) {
            sumk += next[d];
        }

        /* Resize the memo array. */
        seq_t *new_memo = (seq_t *)realloc(memo,
                                           (memo_size + 1) * sizeof(seq_t));
        if (!new_memo) abort();

        /* Construct the new element (C++ only). */
        init_seq_t(&new_memo[memo_size]);

        /* Store the new value. */
        new_memo[memo_size] = sumk;

        /* Update pointers and counters. */
        memo = new_memo;
        memo_size++;

        /* Prepare cur for the next iteration. */
        for (idx_t d = 0; d < 28; ++d) {
            cur[d] = next[d];
        }
    }

    return memo[n];
}
