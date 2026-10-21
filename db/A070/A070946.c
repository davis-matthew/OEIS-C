/*
 * A070946  Number of permutations on n letters that have only cycles of
 *          length 5 or less.
 * https://oeis.org/A070946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + (n-1)*a(n-2) + (n-1)(n-2)*a(n-3)
 *                          + (n-1)(n-2)(n-3)*a(n-4)
 *                          + (n-1)(n-2)(n-3)(n-4)*a(n-5)
 *
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=24.
 *
 * The function computes the values iteratively, using only the last
 * five terms.  No global caching or dynamic reallocation is
 * required, so it works for all defined `seq_t` implementations
 * (built‑in integers or arbitrary‑precision types).
 */

seq_t a(idx_t n)
{
    /* Handle the known initial segment directly. */
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)1;
    case 2: return (seq_t)2;
    case 3: return (seq_t)6;
    case 4: return (seq_t)24;
    default: break;
    }

    /* Sliding window for the last five terms:
     *   a0 = a(n-5), a1 = a(n-4), a2 = a(n-3),
     *   a3 = a(n-2), a4 = a(n-1)  (before step).
     * Begin with the base values for indices 0‑4. */
    seq_t a0 = (seq_t)1,   /* a(0)   */
           a1 = (seq_t)1,   /* a(1)   */
           a2 = (seq_t)2,   /* a(2)   */
           a3 = (seq_t)6,   /* a(3)   */
           a4 = (seq_t)24;  /* a(4)   */

    seq_t val;              /* will hold a(i) during the loop */

    /* Compute terms from 5 up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        /* Products of successive descending integers. */
        seq_t prod1 = (seq_t)(i - 1);                       /* (i-1)          */
        seq_t prod2 = prod1 * (seq_t)(i - 2);               /* (i-1)(i-2)     */
        seq_t prod3 = prod2 * (seq_t)(i - 3);               /* (i-1)(i-2)(i-3)*/
        seq_t prod4 = prod3 * (seq_t)(i - 4);               /* (i-1)(i-2)(i-3)(i-4) */

        /* Recurrence using the five previous terms. */
        val = a4
              + prod1 * a3
              + prod2 * a2
              + prod3 * a1
              + prod4 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }

    return a4;  /* a(n) is now in a4 after the loop */
}
