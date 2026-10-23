/*
 * A152947  a(n) = 1 + (n-2)*(n-1)/2.
 * https://oeis.org/A152947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 + (n-2)*(n-1)/2
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)                       /* a(1) = a(2) = 1 */
        return (seq_t)1;

    idx_t n1 = n - 1;                 /* n-1  */
    idx_t n2 = n - 2;                 /* n-2  */

    /* The product (n-2)*(n-1) is even. Divide the even factor by two
       first to avoid overflow in the intermediate product. */
    seq_t term;
    if ((n2 & 1U) == 0) {             /* n-2 is even */
        term = ((seq_t)(n2 / 2) * (seq_t)n1) + (seq_t)1;
    } else {                          /* n-1 is even */
        term = ((seq_t)(n1 / 2) * (seq_t)n2) + (seq_t)1;
    }

    return term;
}
