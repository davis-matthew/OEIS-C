/*
 * A187036  An eigensequence of A187034.
 * https://oeis.org/A187036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1. For n even > 1 let k be such that n = 2*(2^r –
 *            1) if k=2r (even index) n = 3*2^r – 2 if k=2r+1 (odd index) Then
 *            a(n) = +1 when k is even or k==1, otherwise a(n) = –1. For all
 *            other n, a(n)=0
 * Range:     i8 n <= 511, i16 n <= 1025, i32 n <= 1025, i64 n <= 1025,
 *            i128 n <= 1025, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    /* odd indices >1 are always zero */
    if ((n & 1ULL) && n != 1) return (seq_t)0;

    /* search for the unique r with n = p_k */
    uint64_t pow2 = 2;        /* pow2 = 2^(r+1) for current r (start r=0) */
    for (idx_t r = 0; ; ++r, pow2 <<= 1) {
        /* even index  k = 2*r  :  p_k = 2*(2^r – 1) = pow2 – 2 */
        seq_t pe = (seq_t)pow2 - (seq_t)2;
        if (pe == (seq_t)n) {
            idx_t k = 2 * r;
            return ((k % 2 == 0) || (k == 1)) ? (seq_t)1 : (seq_t)(-1);
        }
        if (pe > (seq_t)n) break;          /* no larger k can match */

        /* odd index   k = 2*r+1  :  p_k = 3*2^r – 2 = 3*(pow2>>1) – 2 */
        seq_t po = (seq_t)3 * (seq_t)(pow2 >> 1) - (seq_t)2;
        if (po == (seq_t)n) {
            idx_t k = 2 * r + 1;
            return ((k % 2 == 0) || (k == 1)) ? (seq_t)1 : (seq_t)(-1);
        }

        /* prevent overflow: pow2 will become 2^(r+2) on next loop */
        if (pow2 > (UINT64_MAX >> 1)) break;
    }
    return (seq_t)0;
}
