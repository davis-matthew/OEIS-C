/*
 * A011526  Stirling numbers of first kind S1(16,n).
 * https://oeis.org/A011526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = Stirling number of the first kind s(16,n)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (so a(1) corresponds to s(16,1)) */

seq_t a(idx_t n)
{
    static seq_t row[17] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* Initialize the first row of the triangle: s(1,1) = 1 */
        row[1] = 1;
        /* Build rows 2 .. 16 using the recurrence
           s(i,k) = s(i-1,k-1) - (i-1)*s(i-1,k)  */
        for (idx_t i = 2; i <= 16; ++i) {
            seq_t mult = (seq_t)(i - 1);          /* (i-1) as seq_t */
            for (idx_t k = i; k >= 1; --k) {
                seq_t old = row[k];
                if (k == 1) {
                    /* s(i,1) = -(i-1)*s(i-1,1) */
                    row[k] = -mult * old;
                } else {
                    /* s(i,k) = s(i-1,k-1) - (i-1)*s(i-1,k) */
                    row[k] = row[k - 1] - mult * old;
                }
            }
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 16)
        return row[n];
    else
        return 0;   /* values beyond 16 are zero for the fixed m=16 case */
}
