/*
 * A086115  Number of 5 X n (0,1) matrices such that each row and each column
 *          is nondecreasing or nonincreasing.
 * https://oeis.org/A086115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*n*(n^4+10*n^3+35*n^2+50*n-36)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use an unsigned 128‑bit integer for safe intermediate arithmetic. */
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 poly = sn * sn * sn * sn       /* n^4          */
                         + 10ULL * sn * sn * sn      /* +10*n^3      */
                         + 35ULL * sn * sn           /* +35*n^2      */
                         + 50ULL * sn                /* +50*n        */
                         - 36ULL;                    /* -36          */
    unsigned __int128 val = sn * poly;               /* n * polynomial */
    val /= 6;                                       /* divide by 6    */
    return (seq_t)val;                              /* cast back to seq_t */
}
