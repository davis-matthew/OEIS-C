/*
 * A123050  Conjectured number of ordered trees on n edges for which the
 *          conjugate and transpose commute.
 * https://oeis.org/A123050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; for n>=2: a(n)=2*( floor((n+1)/3) +
 *            Σ_{k=1}^{⌊(n-2)/8⌋} floor((n-8k-2)/4) )
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset case */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* sum = floor((n+1)/3) + Σ floor((n-8k-2)/4) */
    seq_t sum = ((seq_t)n + (seq_t)1) / (seq_t)3;   /* floor((n+1)/3) */

    /* iterate over k = 1 .. floor((n-2)/8) */
    idx_t maxk = (n - (idx_t)2) / (idx_t)8;
    for (idx_t k = (idx_t)1; k <= maxk; ++k) {
        idx_t d = n - (idx_t)8 * k - (idx_t)2;  /* n - 8k - 2 */
        sum += (seq_t)d / (seq_t)4;           /* floor(d/4) */
    }

    return sum * (seq_t)2;
}
