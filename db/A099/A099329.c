/*
 * A099329  Number of Catalan knight paths from (0,0) to (n,1) in the region
 *          between and on the lines y=0 and y=3. (See A096587 for the
 *          definition of Catalan knight.).
 * https://oeis.org/A099329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n‑1,1) where T(n,y) satisfies T(0,0)=1,
 *            T(0,1)=T(0,2)=T(0,3)=0, and for n≥2 T(n,0)=T(n‑1,2)+T(n‑2,1)
 *            T(n,1)=T(n‑1,3)+T(n‑2,0)+T(n‑2,2)
 *            T(n,2)=T(n‑1,0)+T(n‑2,1)+T(n‑2,3) T(n,3)=T(n‑1,1)+T(n‑2,2)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A099329 */
seq_t a(idx_t n) {
    /* a(1)=0 by definition; the recurrence uses T(n‑1,1) for n≥1 */
    if (n < 2) return 0;          /* a(1)=0, a(2)=0 will be handled by loop */

    idx_t m = n - 1;              /* we need T(m,1) */

    /* T(n,0..3) for two previous columns */
    seq_t prev2[4] = {0, 0, 0, 0}; /* T(n‑2,*)  */
    seq_t prev1[4] = {1, 0, 0, 0}; /* T(n‑1,*)  (T(0,0)=1) */
    seq_t cur[4];

    for (idx_t i = 1; i <= m; ++i) {
        /* compute T(i, *) from prev1 (T(i‑1,*)) and prev2 (T(i‑2,*)) */
        cur[0] = prev1[2] + prev2[1];
        cur[1] = prev1[3] + prev2[0] + prev2[2];
        cur[2] = prev1[0] + prev2[1] + prev2[3];
        cur[3] = prev1[1] + prev2[2];

        /* shift columns for next iteration */
        prev2[0] = prev1[0];
        prev2[1] = prev1[1];
        prev2[2] = prev1[2];
        prev2[3] = prev1[3];

        prev1[0] = cur[0];
        prev1[1] = cur[1];
        prev1[2] = cur[2];
        prev1[3] = cur[3];
    }

    return prev1[1]; /* T(n‑1,1) = a(n) */
}
