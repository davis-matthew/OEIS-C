/*
 * A099331  Number of Catalan knight paths from (0,0) to (n,3) in the region
 *          between and on the lines y=0 and y=3. (See A096587 for the
 *          definition of Catalan knight.).
 * https://oeis.org/A099331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for i ≥ 2
   T(i,0) = T(i-1,2) + T(i-2,1)
   T(i,1) = T(i-1,3) + T(i-2,0) + T(i-2,2)
   T(i,2) = T(i-1,0) + T(i-2,1) + T(i-2,3)
   T(i,3) = T(i-1,1) + T(i-2,2)

   a(n) = T(n,3).  Initial rows:
   T(0,0)=1;   (others 0)
   T(1,2)=1;   (others 0)
   Thus a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=1, a(5)=4, a(6)=3, … */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;   /* T(1,3)=0 */

    /* Tables for rows i-1 and i-2, and the current row */
    seq_t prev[4]  = {0, 0, 1, 0};  /* row for i=1: only T(1,2)=1 */
    seq_t prev2[4] = {1, 0, 0, 0};  /* row for i=0: only T(0,0)=1 */
    seq_t cur[4];

    /* If the requested n is 1 we already have the answer in prev[3] */
    if (n == 1) return prev[3];

    /* Compute rows up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur[0] = prev[2] + prev2[1];
        cur[1] = prev[3] + prev2[0] + prev2[2];
        cur[2] = prev[0] + prev2[1] + prev2[3];
        cur[3] = prev[1] + prev2[2];

        /* Shift rows for next iteration */
        prev2[0] = prev[0]; prev2[1] = prev[1];
        prev2[2] = prev[2]; prev2[3] = prev[3];
        prev[0] = cur[0];   prev[1] = cur[1];
        prev[2] = cur[2];   prev[3] = cur[3];
    }

    return prev[3];   /* a(n) = T(n,3) */
}
