/*
 * A227026  Numbers k such that k!/m! is a triangular number for some m < k-1.
 * https://oeis.org/A227026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7a(n-1) - 7a(n-2) + a(n-3) for n > 14 Initial values
 *            (a(1) to a(15)): 3, 5, 6, 7, 11, 14, 15, 58, 85, 493, 638, 2871,
 *            16731, 97513, 568345
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table of the first 15 terms */
    static const seq_t first[15] = {
        (seq_t)3,    (seq_t)5,    (seq_t)6,    (seq_t)7,
        (seq_t)11,   (seq_t)14,   (seq_t)15,   (seq_t)58,
        (seq_t)85,   (seq_t)493,  (seq_t)638,  (seq_t)2871,
        (seq_t)16731,(seq_t)97513,(seq_t)568345
    };

    if (n <= 15ULL)
        return first[n - 1];

    /* start from a(13), a(14), a(15) */
    seq_t a_nm3 = first[12];   /* a(n-3) for n=16 */
    seq_t a_nm2 = first[13];   /* a(n-2) for n=16 */
    seq_t a_nm1 = first[14];   /* a(n-1) for n=16 */
    seq_t res   = 0;

    for (idx_t i = 16; i <= n; ++i) {
        res = (((seq_t)7) * a_nm1)
              - (((seq_t)7) * a_nm2)
              + a_nm3;
        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }
    return res;
}
