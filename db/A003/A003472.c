/*
 * A003472  a(n) = 2^(n-4)*C(n,4).
 * https://oeis.org/A003472
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n-4)*C(n,4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 50, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    while (e > 0) {
        if (e & 1) {
            result *= base;
        }
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    
    /* Compute C(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    seq_t binom = (nseq * (nseq - 1)) / 2;
    binom = (binom * (nseq - 2)) / 3;
    binom = (binom * (nseq - 3)) / 4;
    
    /* Compute 2^(n-4) */
    seq_t power_of_2 = pow2(n - 4);
    
    return binom * power_of_2;
}
