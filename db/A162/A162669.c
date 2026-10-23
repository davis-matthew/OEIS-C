/*
 * A162669  a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)/5.
 * https://oeis.org/A162669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)/5
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    int divided = 0;                     /* flag indicating division by 5 done */
    for (idx_t i = 0; i < 6; ++i) {
        seq_t term = (seq_t)(n + i);      /* multiset element */
        if (!divided && term % 5 == 0) {
            term /= 5;                    /* divide one factor by 5 */
            divided = 1;
        }
        prod *= term;                     /* accumulate product */
    }
    return prod;
}
