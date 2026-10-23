/*
 * A011527  Stirling numbers of first kind S1(17,n).
 * https://oeis.org/A011527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = Stirling numbers of the first kind s(17,n)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* s(0,0)=1, s(n,0)=0 (n>0), s(0,k)=0 (k>0);  s(n,k)=s(n-1,k-1)-(n-1)*s(n-1,k) */

seq_t a(idx_t n) {
    static int init = 0;
    static seq_t result[18];  /* result[0] unused; result[1..17] = s(17,k) */

    if (!init) {
        seq_t prev[18] = {0};
        seq_t curr[18] = {0};

        prev[0] = 1;  /* s(0,0) = 1 */

        for (idx_t i = 1; i <= 17; ++i) {
            curr[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                curr[k] = prev[k - 1] - (seq_t)(i - 1) * prev[k];
            }
            /* copy curr to prev for next iteration */
            for (idx_t k = 0; k <= 17; ++k) {
                prev[k] = curr[k];
            }
        }

        /* prev now holds s(17,k) for k=0..17 */
        for (idx_t k = 0; k <= 17; ++k) {
            result[k] = prev[k];
        }

        init = 1;
    }

    /* Offset is 1: a(1) = s(17,1) */
    if (n == 0 || n > 17) {
        return 0;
    }
    return result[n];
}
