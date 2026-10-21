/*
 * A224254  Full cycle lengths in the Collatz (3x+1) problem when the negative
 *          integers are used.
 * https://oeis.org/A224254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the (full) cycle that contains -n when the
 *            Collatz (3x+1) map is applied to negative integers
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* The function uses only integer arithmetic, is valid C11
   and also compiles as C++17 (with no class or template usage). */
seq_t a(idx_t n)
{
    /* Convert the index to a signed 64‑bit integer.
       The Collatz iterations stay within a relatively small
       range for n ≤ 10000, so 64 bits are sufficient. */
    int64_t start = -(int64_t)n;
    int64_t cur   = start;

    /* A simple linear search for the first repeated value.
       The maximum number of steps before a cycle is far below 400
       for n ≤ 10000, so a small static table is adequate. */
    enum { MAX_STEPS = 4096 };   /* safety margin */
    int64_t visited[MAX_STEPS];
    int      step_at[MAX_STEPS];
    int      idx = 0;            /* number of stored values */
    int      step = 0;           /* current step number */

    while (1) {
        /* look for a previous occurrence of cur */
        for (int j = 0; j < idx; ++j) {
            if (visited[j] == cur) {
                int cycle_len = step - step_at[j];
                return (seq_t)cycle_len;
            }
        }

        /* store cur before moving on */
        if (idx >= MAX_STEPS) {
            /* guard against pathological growth –
               this branch will never be taken for the intended range */
            return (seq_t)0;
        }
        visited[idx] = cur;
        step_at[idx] = step;
        ++idx;
        ++step;

        /* next Collatz step: even? divide by 2, otherwise 3x+1 */
        if ((cur & 1LL) == 0) {
            cur /= 2;
        } else {
            cur = cur * 3 + 1;
        }
    }
}
