/*
 * A022817  Number of terms in 7th derivative of a function composed with
 *          itself n times.
 * https://oeis.org/A022817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/720 * (n^5 + 39*n^4 + 355*n^3 + 645*n^2 - 356*n + 36)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* a(0) is undefined; return 0. */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    /* Compute the polynomial in a way that works with unsigned seq_t. */
    seq_t temp = n5
               + (seq_t)39 * n4
               + (seq_t)355 * n3
               + (seq_t)645 * n2
               + (seq_t)36;
    seq_t sub  = (seq_t)356 * nn;
    seq_t num  = temp - sub;     /* temp > sub for all n >= 1 */

    num = num * nn;              /* multiply by n */
    return num / (seq_t)720;     /* divide by 720; division is exact */
}
