/*
 * A035614  Horizontal para-Fibonacci sequence: says which column of Wythoff
 *          array (starting column count at 0) contains n+1.
 * https://oeis.org/A035614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035614 – Horizontal para‑Fibonacci sequence
 *
 *  a(n) = number of trailing zeros in the Zeckendorf representation
 *  of (n + 1).  The Zeckendorf representation uses the Fibonacci
 *  numbers F2 = 1, F3 = 2, F4 = 3, … with the condition that no
 *  two consecutive Fibonacci numbers are used.  The binary string of
 *  1's and 0's (bit 0 corresponds to F2) is considered; the
 *  trailing zeros are the number of consecutive zeros starting at the
 *  least‑significant bit before the first 1 bit.  The algorithm below
 *  builds the Fibonacci numbers up to the required value and then
 *  applies the greedy Zeckendorf decomposition, keeping track of the
 *  smallest (least‑significant) Fibonacci used.
 */

#include <stddef.h>
#include <stdbool.h>

/*  seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /*  Number whose Zeckendorf representation we need.  */
    seq_t m = (seq_t)n + (seq_t)1;

    /*  Fibonacci table F2 = 1, F3 = 2, …  */
    static seq_t fib[128];
    static idx_t fibcnt = 0;

    /*  Initialise the first two Fibonacci numbers once.  */
    if (fibcnt == 0) {
        fib[0] = (seq_t)1;     /* F2 */
        fib[1] = (seq_t)2;     /* F3 */
        fibcnt   = 2;
    }

    /*  Grow the table until the last element exceeds m.
     *  We stop both when the table is full or when adding would
     *  overflow the seq_t type.  */
    while (fibcnt < 128 && fib[fibcnt - 1] <= m) {
        seq_t next = fib[fibcnt - 1] + fib[fibcnt - 2];
        /*  Detect unsigned overflow: next < previous means wrap‑around.  */
        if (next < fib[fibcnt - 1]) {
            break;
        }
        fib[fibcnt++] = next;
    }

    /*  Greedy Zeckendorf decomposition – we walk from the largest
     *  Fibonacci downwards, subtracting whenever possible.  We keep
     *  the smallest index used, which equals the number of trailing
     *  zeros in the binary string of the representation.  */
    seq_t rem = m;
    seq_t min_index = (seq_t)-1;      /* sentinel */

    for (idx_t i = fibcnt; i > 0; ) {
        --i;
        if (fib[i] <= rem) {
            rem -= fib[i];
            if (min_index == (seq_t)-1 || (seq_t)i < min_index) {
                min_index = (seq_t)i;
            }
        }
    }

    /*  min_index always becomes non‑negative for m ≥ 1.  */
    return min_index;
}
