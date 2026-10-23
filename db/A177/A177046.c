/*
 * A177046  a(n) = 127*(n-1)-a(n-1) with n>1, a(1)=16.
 * https://oeis.org/A177046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (127-63*(-1)^(n-1)+254*(n-1))/4
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 safely; the sequence is defined for n >= 1. */
    if (n == 0) return 0;

    /* Convert the index difference to seq_t once to avoid repeated casts. */
    seq_t n1 = (seq_t)(n - 1);

    /* Compute the large term separately. Multiplication stays within seq_t range
       for all n that produce a value fitting into seq_t. */
    seq_t term3 = (seq_t)254 * n1;

    /* For odd n (n-1 even) the sum is 64 + term3; for even n it is 190 + term3. */
    seq_t sum;
    if (((n - 1) & 1) == 0) /* n odd */
        sum = (seq_t)64 + term3;
    else                   /* n even */
        sum = (seq_t)190 + term3;

    /* Division by 4 yields the exact integer value. */
    return sum / (seq_t)4;
}
