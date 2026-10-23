/*
 * A193991  Number of zeros in the period of Fibonacci 3-step sequence A000073
 *          mod n.
 * https://oeis.org/A193991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of zeros in the period of the tribonacci sequence
 *            (t_k = t_{k-1}+t_{k-2}+t_{k-3}) modulo n, with t_0=0, t_1=0,
 *            t_2=1
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n = 1: all terms are 0; the period length is 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* ------------------------------------------------------------------
     * Find the period length L.
     * We generate terms until the state (t_{k-2}, t_{k-1}, t_k) repeats
     * the initial state (0,0,1).  The period consists of the first L terms,
     * where L = k-2.
     * ------------------------------------------------------------------ */
    idx_t prev2 = 0;      /* t_{k-2} */
    idx_t prev1 = 0;      /* t_{k-1} */
    idx_t cur   = 1;      /* t_k  (1 % n for n>1) */
    idx_t idx   = 2;      /* current index of cur */
    const idx_t one_mod = 1 % n;   /* equals 1 for n>1 */

    while (1) {
        idx++;                         /* move to next term */
        idx_t next = (prev2 + prev1 + cur) % n;
        prev2 = prev1;
        prev1 = cur;
        cur   = next;

        if (prev2 == 0 && prev1 == 0 && cur == one_mod) {
            break;                     /* initial triple has re‑appeared */
        }
    }

    /* Period length L = idx - 2 (index of the repeating 1 minus 2). */
    idx_t L = idx - 2;

    /* ------------------------------------------------------------------
     * Count zeros among the first L terms (t_0 .. t_{L-1}).
     * We regenerate the sequence once more, but only store the three
     * most recent terms.  No large array is needed.
     * ------------------------------------------------------------------ */
    seq_t zero_count = 0;
    prev2 = 0;
    prev1 = 0;
    cur   = 1;          /* same start as before */

    /* Count the initial two zeros (t_0 and t_1). */
    zero_count++;  /* t_0 == 0 */
    zero_count++;  /* t_1 == 0 */

    /* t_2 may be zero only when n == 1, but n > 1 here. */
    /* (The test for cur==0 is kept for completeness.) */
    if (cur == 0) {
        zero_count++;
    }

    for (idx = 3; idx < L; idx++) {
        idx_t next = (prev2 + prev1 + cur) % n;
        prev2 = prev1;
        prev1 = cur;
        cur   = next;

        if (cur == 0) {
            zero_count++;
        }
    }

    return zero_count;
}
