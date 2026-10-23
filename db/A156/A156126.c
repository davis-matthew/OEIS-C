/*
 * A156126  Sequence related to Hankel transform of super-ballot numbers.
 * https://oeis.org/A156126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+5)*(2*n+3)*(n+2)/3 for n>0; a(0)=1
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t aTerm = (seq_t)n * 2 + 5;   /* 2*n+5 */
    seq_t bTerm = (seq_t)n * 2 + 3;   /* 2*n+3 */
    seq_t cTerm = (seq_t)n + 2;       /* n+2 */
    /* divide the product by 3 exactly */
    if (aTerm % (seq_t)3 == 0) {
        aTerm /= 3;
    } else if (bTerm % (seq_t)3 == 0) {
        bTerm /= 3;
    } else {
        cTerm /= 3;
    }
    seq_t prod = aTerm * bTerm;
    prod = prod * cTerm;
    return prod;
}
