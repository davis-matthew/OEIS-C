/*
 * A078344  a(1)=1; a(2)=2; a(3)=3; a(n) = Sum_{k=3..n-1} (a(k) + a(k-1) +
 *          a(k-2)).
 * https://oeis.org/A078344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A078344
   Recurrence:  a(1)=1, a(2)=2, a(3)=3, a(4)=6;
                for n>4:  a(n)=2*a(n-1)+a(n-2)+a(n-3)  */

/* The function is memoised; it works for any order of calls.
   When seq_t is an arbitrary‑precision class, realloc is cast safely
   using a C‑style cast which is accepted by both C11 and C++17. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;   /* memoised values, 0‑based array  */
    static size_t memo_len = 0;  /* number of stored terms          */

    if (n == 0) return 0; /* not part of the sequence, safe guard */

    size_t required = (size_t)n;   /* n ≥ 1, fits into size_t */

    if (required > memo_len) {
        size_t old_len = memo_len;
        size_t new_len = required;

        /* allocate or grow the array (C and C++ compatible cast) */
        seq_t *new_mem = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!new_mem) return 0;          /* allocation failure – unlikely */

        memo = new_mem;

        /* initialise the first few terms if they are not yet set */
        if (old_len < 1) {
            if (new_len > 0) memo[0] = 1;   /* a(1) */
        }
        if (old_len < 2) {
            if (new_len > 1) memo[1] = 2;   /* a(2) */
        }
        if (old_len < 3) {
            if (new_len > 2) memo[2] = 3;   /* a(3) */
        }
        if (old_len < 4) {
            if (new_len > 3) memo[3] = 6;   /* a(4) */
        }

        /* compute missing terms using the recurrence */
        size_t start = old_len < 4 ? 4 : old_len;
        for (size_t i = start; i < new_len; ++i) {
            memo[i] = 2 * memo[i-1] + memo[i-2] + memo[i-3];
        }

        memo_len = new_len;
    }

    return memo[required - 1];  /* a(n) stored at index n-1 */
}
