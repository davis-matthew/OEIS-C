/*
 * A095311  47-gonal numbers.
 * https://oeis.org/A095311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(45*n-43)/2 (for n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1.  For n == 0 the result is not used. */
    seq_t ns = (seq_t)n;                     /* cast index to seq_t */
    seq_t term = ((seq_t)45 * ns) - (seq_t)43; /* 45*n - 43 */

    /* Divide the product by 2 in the safe order depending on parity
       to avoid truncation of an odd intermediate. */
    if ((n & 1u) == 0) {                    /* n even -> term is odd */
        seq_t half_n = ns / (seq_t)2;
        return half_n * term;
    } else {                                /* n odd -> term is even */
        seq_t half_term = term / (seq_t)2;
        return ns * half_term;
    }
}
