/*
 * A216243  Partial sums of the squares of Lucas numbers (A000032).
 * https://oeis.org/A216243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(2*n+1) + 2 + (-1)^n (Lucas sequence L(0)=2, L(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the Lucas number L(2*n+1) iteratively */
    idx_t m = (idx_t)2 * n + (idx_t)1;   /* m = 2*n+1, fits in idx_t */
    seq_t Lprev = (seq_t)2;   /* L(0) */
    seq_t Lcurr = (seq_t)1;   /* L(1) */

    if (m < (idx_t)2) {       /* n == 0 → m == 1 */
        /* L(1) is already stored in Lcurr */
    } else {
        for (idx_t i = (idx_t)2; i <= m; ++i) {
            seq_t Lnext = Lprev + Lcurr;  /* L(i) = L(i-1)+L(i-2) */
            Lprev = Lcurr;
            Lcurr = Lnext;
        }
    }

    /* Now Lcurr == L(2*n+1) */
    seq_t result = Lcurr + ((seq_t)2);
    if ((n & (idx_t)1) == (idx_t)0) {
        /* even n → (+1) */
        result += (seq_t)1;
    } else {
        /* odd n → (-1) */
        result -= (seq_t)1;
    }

    return result;
}
