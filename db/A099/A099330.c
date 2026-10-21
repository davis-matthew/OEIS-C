/*
 * A099330  Number of Catalan knight paths from (0,0) to (n,2) in the region
 *          between and on the lines y=0 and y=3. (See A096587 for the
 *          definition of Catalan knight.).
 * https://oeis.org/A099330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS A099330: 
 * a(n) = number of Catalan knight paths from (0,0) to (n,2) in the strip 0 ≤ y ≤ 3.
 * The values are obtained from the recurrence for the table
 *   T(x,y)  = number of ways to reach coordinate (x,y).
 *   T(0,0)=1,  T(0,1)=T(0,2)=T(0,3)=0
 *   For x≥2:
 *       T(x,0) = T(x-1,2) + T(x-2,1)
 *       T(x,1) = T(x-1,3) + T(x-2,0) + T(x-2,2)
 *       T(x,2) = T(x-1,0) + T(x-2,1) + T(x-2,3)
 *       T(x,3) = T(x-1,1) + T(x-2,2)
 *   The desired a(n) equals T(n-1,2).  The first column x=1 has only one
 *   path to (1,2), hence T(1,2)=1.
 * 
 * Loop over the columns up to n-1, keeping only the last two columns.
 * The algorithm runs in O(n) time and O(1) memory.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by the problem statement.  Work on x = n-1. */
    idx_t target = n - 1;

    /* T(0,0)=1, all others 0.  prev_prev is T(x-2,·). */
    seq_t prev_prev[4] = {0};
    prev_prev[0] = 1;                     /* T(0,0) */

    /* T(1,2)=1, others 0.  prev is T(x-1,·). */
    seq_t prev[4] = {0};
    prev[2] = 1;                           /* T(1,2) */

    if (target == 0)           return prev_prev[2];  /* a(1) */
    if (target == 1)           return prev[2];       /* a(2) */

    /* Compute columns 2 … target.  cur = T(x,·). */
    for (idx_t x = 2; x <= target; ++x) {
        seq_t cur[4];

        cur[0] = prev[2] + prev_prev[1];
        cur[1] = prev[3] + prev_prev[0] + prev_prev[2];
        cur[2] = prev[0] + prev_prev[1] + prev_prev[3];
        cur[3] = prev[1] + prev_prev[2];

        /* shift: prev_prev <- prev, prev <- cur */
        prev_prev[0] = prev[0];
        prev_prev[1] = prev[1];
        prev_prev[2] = prev[2];
        prev_prev[3] = prev[3];

        prev[0] = cur[0];
        prev[1] = cur[1];
        prev[2] = cur[2];
        prev[3] = cur[3];
    }

    return prev[2];   /* T(target,2) = a(n) */
}
