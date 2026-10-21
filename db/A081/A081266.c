/*
 * A081266  Staggered diagonal of triangular spiral in A051682.
 * https://oeis.org/A081266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3*n+1)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    seq_t n3 = nn * (seq_t)3;      /* 3*n */

    /* Perform division by 2 before a costly multiplication to avoid overflow */
    seq_t result;
    if (n3 % (seq_t)2 == 0) {
        result = (n3 / (seq_t)2) * (n3 + (seq_t)1);
    } else {
        result = n3 * ((n3 + (seq_t)1) / (seq_t)2);
    }
    return result;
}
